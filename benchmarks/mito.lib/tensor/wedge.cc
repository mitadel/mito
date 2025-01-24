// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// get the benchmark library
#include <benchmark/benchmark.h>

// get tensor
#include <mito/tensor.h>


auto
wedge_array(const auto & a, const auto & b, const auto & xi1, const auto & xi2)
{
    // all done
    return (a[0] * xi1[0] + a[1] * xi1[1] + a[2] * xi1[2])
             * (b[0] * xi2[0] + b[1] * xi2[1] + b[2] * xi2[2])
         - (b[0] * xi1[0] + b[1] * xi1[1] + b[2] * xi1[2])
               * (a[0] * xi2[0] + a[1] * xi2[1] + a[2] * xi2[2]);
}


auto
wedge_mito(const auto & two_form, const auto & xi1, const auto & xi2)
{
    // all done
    return two_form(xi1, xi2);
}

static void
WedgeArray(benchmark::State & state)
{
    // a vector representing a one-form
    std::array<double, 3> a = { 2.0, 1.0, 0.0 };

    // a vector representing another one-form
    std::array<double, 3> b = { 0.0, 0.0, 1.0 };

    // a vector
    std::array<double, 3> xi1 = { 0.0, 0.0, 1.0 };

    // another vector
    std::array<double, 3> xi2 = { 0.0, 1.0, 0.0 };

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(wedge_array(a, b, xi1, xi2));
    }
}

static void
WedgeMito(benchmark::State & state)
{
    // the Euclidean metric in 3D space
    constexpr auto metric = mito::identity<3>;

    // a vector field
    auto a = 2.0 * mito::e_0<3> + mito::e_1<3>;

    // the corresponding one-form
    auto a_tilda = mito::tensor::one_form(a, metric);

    // another vector field
    auto b = mito::e_2<3>;

    // the corresponding one-form
    auto b_tilda = mito::tensor::one_form(b, metric);

    // the wedge product of {a_tilda} and {b_tilda} (a two-form)
    auto ab_two_form = mito::tensor::wedge(a_tilda, b_tilda);

    // a vector
    auto xi1 = mito::e_2<3>;

    // another vector
    auto xi2 = mito::e_1<3>;

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(wedge_mito(ab_two_form, xi1, xi2));
    }
}


// run benchmark for 3D wedge products (array)
BENCHMARK(WedgeArray);
// run benchmark for 3D wedge products (mito)
BENCHMARK(WedgeMito);


// run all benchmarks
BENCHMARK_MAIN();


// end of file
