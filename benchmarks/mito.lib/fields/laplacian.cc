// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>

// get the mito materials
#include <mito/fields.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


auto
random_coordinate()
{
    // seed the random number generator
    std::srand(time(0));
    // a truly random number
    auto rand_num = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    // a point in 2D space
    return mito::geometry::coordinates<coordinates_t>({ rand_num, rand_num });
}


auto
laplacian_baseline(const coordinates_t & x)
{
    // evaluate the laplacian at {x}
    auto result =
        12 * x[0] * x[0] * x[1] * x[1] * x[1] * x[1] + 12 * x[1] * x[1] * x[0] * x[0] * x[0] * x[0];

    // all done
    return result;
}


auto
laplacian_mito(const coordinates_t & x)
{
    // the function extracting the x_0 component of 2D vector
    auto x0 = mito::functions::component<coordinates_t, 0>;
    // the function extracting the x_1 component of a 2D vector
    auto x1 = mito::functions::component<coordinates_t, 1>;

    // a scalar field
    auto f = mito::fields::field(mito::functions::pow<4>(x0 * x1));

    // the gradient of {f}
    auto gradient = mito::fields::gradient(f);

    // the laplacian (divergence of gradient)
    auto laplacian = mito::fields::divergence(gradient);

    // evaluate the laplacian at {x}
    auto result = laplacian(x);

    // all done
    return result;
}


static void
LaplacianBaseline(benchmark::State & state)
{
    // generate a set of random coordinates
    auto x = random_coordinate();

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(laplacian_baseline(x));
    }
}

static void
LaplacianMito(benchmark::State & state)
{
    // generate a set of random coordinates
    auto x = random_coordinate();

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(laplacian_mito(x));
    }
}


// run benchmark for laplacian calculation (baeline)
BENCHMARK(LaplacianBaseline);
// run benchmark for laplacian calculation (mito)
BENCHMARK(LaplacianMito);


// run all benchmarks
BENCHMARK_MAIN();


// end of file
