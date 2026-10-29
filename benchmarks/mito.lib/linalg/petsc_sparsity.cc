// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the shared benchmark support
#include "utilities/petsc_assembly.h"


// the benchmark support
using namespace mito::benchmarks::linalg::petsc;


int
main(int argc, char ** argv)
{
    // initialize PETSc
    auto environment = mito::linalg::petsc::environment();

    // initialize the benchmark library
    benchmark::Initialize(&argc, argv);

    // the problem size and number of iterations
    const auto [nx, iterations] = options();
    const index_t N = n_equations(nx);

    // the stiffness matrix K
    auto K = mito::linalg::petsc::matrix("K", N, format_t::coo);

    // assemble K from the stiffness element
    assemble_from(K, grid_lists(nx, stiffness_element));

    // the consistent mass matrix M: the same pattern as K
    auto M = mito::linalg::petsc::matrix("M", N, format_t::coo);

    // assemble M from the mass element
    assemble_from(M, grid_lists(nx, mass_element));

    // the lumped mass matrix L; diagonal matrix
    auto L = mito::linalg::petsc::matrix("L", N, format_t::coo);

    // assemble L from the lumped mass element
    assemble_from(L, lumped_lists(nx, 9.0));

    // the scaling of the added matrix, e.g. 1 / dt
    const scalar_t alpha = 10.0;

    // create a std::array mapping the declared sparsity pattern relations to their names
    const std::array<std::pair<std::string, mito::linalg::petsc::sparsity_pattern>, 3>
        copy_cases = { { { "same", mito::linalg::petsc::sparsity_pattern::same },
                         { "different", mito::linalg::petsc::sparsity_pattern::different },
                         { "unknown", mito::linalg::petsc::sparsity_pattern::unknown } } };

    // loop over the declared sparsity pattern relations
    for (const auto & [case_name, pattern] : copy_cases) {
        // the declared sparsity pattern relation for this benchmark
        const auto declared = pattern;

        // name of the benchmark
        std::string name = std::string("copy/") + case_name;

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(name, iterations, [&K, N, declared](benchmark::State & state) {
            for (auto _ : state) {
                // a fresh matrix
                auto Keff = mito::linalg::petsc::matrix("Keff", N);

                // copy the structure of K into Keff (not timed)
                Keff.duplicate(K, mito::linalg::petsc::duplicate_policy::copy_pattern);

                // time the copy of the values from K into Keff
                state.SetIterationTime(timed([&] { Keff.copy_values(K, declared); }));
            }
        });
    }

    // the declared sparsity pattern
    struct case_t {
        std::string name;
        const mito::linalg::petsc::Matrix & matrix;
        mito::linalg::petsc::sparsity_pattern pattern;
    };

    const std::array<case_t, 6> axpy_cases = {
        { { "same", M, mito::linalg::petsc::sparsity_pattern::same },
          { "different", M, mito::linalg::petsc::sparsity_pattern::different },
          { "unknown", M, mito::linalg::petsc::sparsity_pattern::unknown },
          { "subset_lumped", L, mito::linalg::petsc::sparsity_pattern::subset },
          { "different_lumped", L, mito::linalg::petsc::sparsity_pattern::different },
          { "unknown_lumped", L, mito::linalg::petsc::sparsity_pattern::unknown } }
    };

    // loop over the declared sparsity pattern relations for MatAXPY
    for (const auto & [case_name, matrix, pattern] : axpy_cases) {
        // the declared sparsity pattern relation for this benchmark
        const auto declared = pattern;

        // name of the benchmark
        std::string name = std::string("axpy/") + case_name;

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(
            name, iterations, [&K, &matrix, N, alpha, declared](benchmark::State & state) {
                for (auto _ : state) {
                    // a fresh copy of K
                    auto Keff = mito::linalg::petsc::matrix("Keff", N);

                    // copy the structure of K into Keff (not timed)
                    Keff.duplicate(K, mito::linalg::petsc::duplicate_policy::copy_pattern);

                    // copy the values of K into Keff (not timed)
                    Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);

                    // time the addition
                    state.SetIterationTime(timed([&] { Keff.add(alpha, matrix, declared); }));
                }
            });
    }

    // run the benchmarks; only rank 0 reports
    run_benchmarks();

    // finalize the benchmark library
    benchmark::Shutdown();

    // all done
    return 0;
}


// end of file