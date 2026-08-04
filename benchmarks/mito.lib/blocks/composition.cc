// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>
// get the mito library
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;

// simplicial cells in 2D
using cell_t = mito::geometry::triangle_t<2>;
// second degree finite elements
constexpr int degree = 2;
// assemble the finite element type
using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;


auto
block_composition(const auto & sum_block, const auto & function_space)
{
    // a matrix to collect the elementary contributions of the block
    auto elementary_block_sum =
        typename mito::utilities::base_type<decltype(sum_block)>::elementary_shape{};

    // loop over the elements of the function space and collect the block elementary contributions
    for (auto element : function_space.elements()) {
        elementary_block_sum += sum_block.compute(element);
    }

    // all done
    return elementary_block_sum;
}

auto
block_no_composition(const auto & block_1, const auto & block_2, const auto & function_space)
{
    // a matrix to collect the elementary contributions of the block
    auto elementary_block_sum =
        typename mito::utilities::base_type<decltype(block_1)>::elementary_shape{};

    // loop over the elements of the function space and collect the block elementary contributions
    for (auto element : function_space.elements()) {
        // compute the elementary contribution of the first block
        auto block_1_contribution = block_1.compute(element);
        // compute the elementary contribution of the second block
        auto block_2_contribution = block_2.compute(element);
        // add them up
        elementary_block_sum += 2.0 * block_1_contribution - block_2_contribution;
    }

    // all done
    return elementary_block_sum;
}

int
main(int argc, char ** argv)
{
    // initialize the benchmark library
    benchmark::Initialize(&argc, argv);

    // make a channel
    journal::info_t channel("blocks.composition");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a square in 2D
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, coord_system);

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system, metric_space_t::w);

    // get the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the zero field
    auto zero = mito::functions::zero<coordinates_t>;

    // set homogeneous Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // the function space (linear elements on the manifold)
    auto function_space = mito::fem::function_space<finite_element_t>(manifold, constraints);

    // a mass matrix block
    constexpr auto mass_block = mito::fem::blocks::mass_block<finite_element_t>();

    // the diffusivity field
    auto diffusivity = mito::functions::identity<coordinates_t, 2>();

    // a grad-grad matric block
    auto grad_grad_block = mito::fem::blocks::grad_grad_block<finite_element_t>(diffusivity);

    // add them up
    auto sum_block = 2.0 * mass_block + (-1.0) * grad_grad_block;

    // run the benchmark for the block composition
    benchmark::RegisterBenchmark("blocks/composition", [&](benchmark::State & state) {
        for (auto _ : state) {
            // collect the block elementary contributions over the function space elements
            auto result = block_composition(sum_block, function_space);
            // do not optimize the result away
            benchmark::DoNotOptimize(result);
            // // show the result
            // channel << "result with composition = " << result << journal::endl;
        }
    });

    // run the benchmark for the block with no composition
    benchmark::RegisterBenchmark("blocks/no_composition", [&](benchmark::State & state) {
        for (auto _ : state) {
            // collect the block elementary contributions over the function space elements
            auto result = block_no_composition(mass_block, grad_grad_block, function_space);
            // do not optimize the result away
            benchmark::DoNotOptimize(result);
            // // show the result
            // channel << "result no composition = " << result << journal::endl;
        }
    });

    // run the benchmarks
    benchmark::RunSpecifiedBenchmarks();

    // finalize the benchmark library
    benchmark::Shutdown();

    // all done
    return 0;
}

// end of file
