// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the shared benchmark support
#include "utilities/petsc_assembly.h"


// the benchmark support
using namespace mito::benchmarks::linalg::petsc;


// the cost of updating Keff = K + M / dt after a change of dt: with MatCopy and MatAXPY on
// assembled matrices, or by combining the value arrays per entry and setting them in one go
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

    // the time step
    const scalar_t dt = 0.1;

    // the assembly input of K and M
    const auto listsK = grid_lists(nx, stiffness_element);
    const auto listsM = grid_lists(nx, mass_element);

    // K and M are kept as assembled matrices and Keff is updated with MatAXPY
    register_timed("keff/axpy", iterations, [&](benchmark::State & state) {
        // the assembled K and M, and Keff with K's pattern (not timed)
        auto K = mito::linalg::petsc::matrix("K", N, format_t::coo);
        assemble_from(K, listsK);
        auto M = mito::linalg::petsc::matrix("M", N, format_t::coo);
        assemble_from(M, listsM);
        auto Keff = mito::linalg::petsc::matrix("Keff", N);
        Keff.copy_structure(K, mito::linalg::petsc::duplicate_policy::share_pattern);

        for (auto _ : state) {
            // time one update of Keff
            state.SetIterationTime(timed([&] {
                Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);
                Keff.add(1.0 / dt, M, mito::linalg::petsc::sparsity_pattern::same);
            }));
        }
    });

    // route 2: only the value arrays of K and M are kept; Keff has its own pattern and is
    // updated by combining the arrays per entry and setting the result, for each format
    for (const auto & [name, format] : formats) {
        const auto fmt = format;
        register_timed(
            std::string("keff/element/") + name, iterations, [&, fmt](benchmark::State & state) {
                // Keff with K's pattern, assembled once (not timed)
                auto Keff = mito::linalg::petsc::matrix("Keff", N, fmt);
                Keff.set_pattern(listsK.rows, listsK.cols, listsK.segments);

                // the combined value array
                std::vector<scalar_t> vals(std::size(listsK.vals));
                auto combine = [&] {
                    for (std::size_t k = 0; k < std::size(vals); ++k) {
                        vals[k] = listsK.vals[k] + listsM.vals[k] / dt;
                    }
                };

                combine();
                Keff.set_values(vals);
                Keff.assemble();

                for (auto _ : state) {
                    // time one update of Keff
                    state.SetIterationTime(timed([&] {
                        combine();
                        Keff.set_values(vals);
                        Keff.assemble();
                    }));
                }
            });
    }

    // recomputed element matrices: every step computes the element values anew
    for (const auto & [name, format] : formats) {
        const auto fmt = format;

        // assemble K and M, then Keff = K + M / dt with MatAXPY
        register_timed(
            std::string("recompute/separate/") + name, iterations,
            [&, fmt](benchmark::State & state) {
                // K and M with their patterns, Keff sharing K's pattern (not timed)
                auto K = mito::linalg::petsc::matrix("K", N, fmt);
                assemble_from(K, listsK);
                auto M = mito::linalg::petsc::matrix("M", N, fmt);
                assemble_from(M, listsM);
                auto Keff = mito::linalg::petsc::matrix("Keff", N);
                Keff.copy_structure(K, mito::linalg::petsc::duplicate_policy::share_pattern);

                // the value arrays of K and M
                std::vector<scalar_t> valsK(std::size(listsK.vals));
                std::vector<scalar_t> valsM(std::size(listsM.vals));

                for (auto _ : state) {
                    // time one step
                    state.SetIterationTime(timed([&] {
                        compute_values(nx, 1.0, 0.0, valsK);
                        compute_values(nx, 0.0, 1.0, valsM);
                        K.set_values(valsK);
                        K.assemble();
                        M.set_values(valsM);
                        M.assemble();
                        Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);
                        Keff.add(1.0 / dt, M, mito::linalg::petsc::sparsity_pattern::same);
                    }));
                }
            });

        // compute Ke + Me / dt per element and assemble Keff directly
        register_timed(
            std::string("recompute/combined/") + name, iterations,
            [&, fmt](benchmark::State & state) {
                // Keff with its pattern (not timed)
                auto Keff = mito::linalg::petsc::matrix("Keff", N, fmt);
                assemble_from(Keff, listsK);

                // the value array of Keff
                std::vector<scalar_t> vals(std::size(listsK.vals));

                for (auto _ : state) {
                    // time one step
                    state.SetIterationTime(timed([&] {
                        compute_values(nx, 1.0, 1.0 / dt, vals);
                        Keff.set_values(vals);
                        Keff.assemble();
                    }));
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