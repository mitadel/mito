// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <mito.h>

#include <array>
#include <utility>
#include <vector>


namespace mito::tests::linalg::petsc {

    // types
    using index_t = mito::linalg::petsc::index_t;
    using scalar_t = mito::linalg::petsc::scalar_t;
    using format_t = mito::linalg::assembly_format;
    using segment_t = mito::linalg::block_segment_t;

    // the assembly formats under test
    constexpr std::array<std::pair<const char *, format_t>, 3> formats = { {
        { "coo", format_t::coo },
        { "csr", format_t::csr },
        { "hash", format_t::hash },
    } };

    // an element: its equation numbers (-1 = constrained) and its row-major element matrix
    struct element_t {
        std::vector<index_t> eqs;
        std::vector<scalar_t> Ke;
    };

    // the flat assembly input of one rank
    struct lists_t {
        std::vector<index_t> rows;
        std::vector<index_t> cols;
        std::vector<scalar_t> vals;
        std::vector<segment_t> segments;
    };

    // the rank of this process and the number of ranks
    inline auto rank_and_size() -> std::pair<int, int>
    {
        // the rank and size of this process
        int rank, size;

        // delegate to MPI
        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
        MPI_Comm_size(PETSC_COMM_WORLD, &size);

        // all done
        return { rank, size };
    }

    // a hand-made system with 5 dofs: two 2-node and two 3-node elements
    inline auto test_elements() -> std::vector<element_t>
    {
        // the elements to be assembled, in global order
        return {
            // a symmetric element matrix
            { { 0, 1 }, { 2, -1, -1, 2 } },
            // non-symmetric element matrix
            { { 1, 2 }, { 3, -2, -1, 4 } },
            // a constrained node
            { { 2, 3, -1 }, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } },
            // an explicit zero in the element matrix
            { { 3, 4, 0 }, { 1, 0, 2, 0, 3, 0, 4, 0, 5 } },
        };
    }

    // extract the elements that this rank owns (round robin)
    inline auto local_elements(const std::vector<element_t> & elements) -> std::vector<element_t>
    {
        // the rank and size of this process
        auto [rank, size] = rank_and_size();

        // the elements owned by this rank
        std::vector<element_t> local;

        // round-robin distribution of the elements
        for (std::size_t k = 0; k < std::size(elements); ++k) {
            if (static_cast<int>(k % size) == rank) {
                local.push_back(elements[k]);
            }
        }

        // all done
        return local;
    }

    // obtain the matrix size
    inline auto n_equations(const std::vector<element_t> & elements) -> index_t
    {
        // the number of equations
        index_t local_max = -1;

        // the largest equation number in the elements, plus one
        for (const auto & [eqs, Ke] : elements) {
            for (const auto i : eqs) {
                if (i >= 0) {
                    local_max = std::max(local_max, i + 1);
                }
            }
        }

        // reduce the local maximum to the global maximum
        index_t global_max;
        MPI_Allreduce(&local_max, &global_max, 1, MPIU_INT, MPI_MAX, PETSC_COMM_WORLD);

        // all done
        return global_max;
    }

    // flatten the elements owned by this rank into COO lists and block segments;
    // the element matrices are scaled by {factor} (to enable testing of re-assembly with different
    // values)
    inline auto flatten(const std::vector<element_t> & elements, scalar_t factor) -> lists_t
    {
        // the lists to be returned
        lists_t lists;

        // flatten the elements into COO lists and block segments
        for (std::size_t k = 0; k < std::size(elements); ++k) {
            // the equation numbers and element matrix of this element
            const auto & [eqs, Ke] = elements[k];

            // the number of equations in this element
            const auto n = std::size(eqs);

            // flatten the element into COO lists
            for (std::size_t a = 0; a < n; ++a) {
                for (std::size_t b = 0; b < n; ++b) {
                    lists.rows.push_back(eqs[a]);
                    lists.cols.push_back(eqs[b]);
                    lists.vals.push_back(factor * Ke[a * n + b]);
                }
            }

            // extend the last segment if the block size matches, else start a new one
            if (!std::empty(lists.segments) && lists.segments.back().block_size == n) {
                ++lists.segments.back().n_blocks;
            } else {
                lists.segments.push_back({ n, 1 });
            }
        }

        // all done
        return lists;
    }

    // the reference: the same elements assembled entry by entry with {add_value}
    template <class matrixT>
    inline auto fill_reference(matrixT & R, const std::vector<element_t> & elements, scalar_t factor)
        -> void
    {
        // assemble the elements entry by entry
        for (std::size_t k = 0; k < std::size(elements); ++k) {
            // the equation numbers and element matrix of this element
            const auto & [eqs, Ke] = elements[k];

            // the number of equations in this element
            const auto n = std::size(eqs);

            // add the element to the reference matrix
            for (std::size_t a = 0; a < n; ++a) {
                for (std::size_t b = 0; b < n; ++b) {
                    // only add entries that are not constrained
                    if (eqs[a] >= 0 && eqs[b] >= 0) {
                        R.add_value(eqs[a], eqs[b], factor * Ke[a * n + b]);
                    }
                }
            }
        }

        // assemble the reference matrix
        R.assemble();

        // all done
        return;
    }

    // compare all entries in the rows owned by this rank
    template <class matrixA, class matrixB>
    inline auto expect_equal(const matrixA & A, const matrixB & B, index_t N) -> void
    {
        // the rows owned by this rank
        index_t rstart, rend;

        // delegate to PETSc
        MatGetOwnershipRange(A.get(), &rstart, &rend);

        // compare all entries in the owned rows
        for (index_t i = rstart; i < rend; ++i) {
            for (index_t j = 0; j < N; ++j) {
                // compare the entries of the two matrices and report the row and column of any
                // mismatch
                EXPECT_NEAR(A.get_value(i, j), B.get_value(i, j), 1e-12)
                    << "entry (" << i << ", " << j << ")";
            }
        }

        // all done
        return;
    }

    // check one entry, on the rank that owns its row
    template <class matrixT>
    inline auto expect_entry(const matrixT & A, index_t i, index_t j, scalar_t expected) -> void
    {
        // the rows owned by this rank
        index_t rstart, rend;

        // delegate to PETSc
        MatGetOwnershipRange(A.get(), &rstart, &rend);

        if (rstart <= i && i < rend) {
            // compare the entry of the matrix and report the row and column of any mismatch
            EXPECT_NEAR(A.get_value(i, j), expected, 1e-12) << "entry (" << i << ", " << j << ")";
        }

        // all done
        return;
    }

    // run {f} and return the elapsed wall time, as seen by the slowest rank
    template <class F>
    inline auto timed(F && f) -> PetscLogDouble
    {
        // the elapsed wall time, as seen by the slowest rank
        PetscLogDouble t0, t1;

        // delegate to PETSc and MPI
        MPI_Barrier(PETSC_COMM_WORLD);
        PetscTime(&t0);

        // run the function
        f();

        // delegate to PETSc and MPI
        MPI_Barrier(PETSC_COMM_WORLD);
        PetscTime(&t1);

        // all done
        return t1 - t0;
    }

}    // namespace
