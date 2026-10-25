// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the shared benchmark support
#include "utilities/petsc_assembly.h"


// the benchmark support
using namespace mito::benchmarks::linalg::petsc;


// the cost of MatCopy and MatAXPY for each declared sparsity pattern relation
int
main(int argc, char ** argv)
{
    // initialize PETSc (and MPI)
    auto environment = mito::linalg::petsc::environment();

    // initialize the benchmark library
    benchmark::Initialize(&argc, argv);

    // the problem size and number of iterations
    const auto [nx, iterations] = options();
    const index_t N = n_equations(nx);

    // the stiffness matrix K
    auto K = mito::linalg::petsc::matrix("K", N, format_t::coo);

    // assemble K from the stiffness element, using the grid_lists helper
    assemble_from(K, grid_lists(nx, stiffness_element));

    // the consistent mass matrix M: the same pattern as K
    auto M = mito::linalg::petsc::matrix("M", N, format_t::coo);

    // assemble M from the mass element, using the grid_lists helper
    assemble_from(M, grid_lists(nx, mass_element));

    // the scaling of the added matrix, e.g. 1 / dt
    const scalar_t alpha = 10.0;

    // create a std::array mapping the declared sparsity pattern relations to their names
    const std::array<std::pair<std::string, mito::linalg::petsc::sparsity_pattern>, 2>
        copy_cases = { {
            { "same", mito::linalg::petsc::sparsity_pattern::same },
            { "different", mito::linalg::petsc::sparsity_pattern::different },
        } };

    // loop over the declared sparsity pattern relations
    for (const auto & [case_name, pattern] : copy_cases) {
        // the declared sparsity pattern relation for this benchmark
        const auto declared = pattern;

        // name of the benchmark, e.g. "copy/same" or "copy/different"
        std::string name = std::string("copy/") + case_name;

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(name, iterations, [&K, N, declared](benchmark::State & state) {
            for (auto _ : state) {
                // a fresh matrix
                auto Keff = mito::linalg::petsc::matrix("Keff", N);

                // copy the structure of K into Keff (not timed)
                Keff.copy_structure(K);

                // time the copy of the values from K into Keff, with the declared sparsity pattern
                state.SetIterationTime(timed([&] { Keff.copy_values(K, declared); }));
            }
        });
    }

    // the declared sparsity pattern relations for MatAXPY
    const std::array<std::pair<std::string, mito::linalg::petsc::sparsity_pattern>, 2>
        axpy_cases = { {
            // consistent mass: identical patterns
            { "same", mito::linalg::petsc::sparsity_pattern::same },
            { "different", mito::linalg::petsc::sparsity_pattern::different },
        } };

    // loop over the declared sparsity pattern relations for MatAXPY
    for (const auto & [case_name, pattern] : axpy_cases) {
        // the declared sparsity pattern relation for this benchmark
        const auto declared = pattern;

        // name of the benchmark, e.g. "axpy/same" or "axpy/different"
        std::string name = std::string("axpy/") + case_name;

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(name, iterations, [&K, &M, N, alpha, declared](benchmark::State & state) {
            for (auto _ : state) {
                // a fresh copy of K
                auto Keff = mito::linalg::petsc::matrix("Keff", N);

                // copy the structure of K into Keff (not timed)
                Keff.copy_structure(K);

                // copy the values of K into Keff (not timed)
                Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);

                // time the addition
                state.SetIterationTime(timed([&] { Keff.add(alpha, M, declared); }));
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