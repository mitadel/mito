// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>

// get array
#include <array>

// get the mito materials
#include <mito/materials.h>


// material parameters
constexpr static double rho = 1.0;
constexpr static double kappa = 1.0;
constexpr static double mu = 1.0;
constexpr static double Jm = 1.0;


auto
gent_array(const auto & epsilon)
{
    // build a deformation gradient
    auto F = std::array<double, 9>{ 1.0 + epsilon, epsilon, epsilon, epsilon,      1.0 + epsilon,
                                    epsilon,       epsilon, epsilon, 1.0 + epsilon };


    double f11 = F[0 * 3 + 0];
    double f12 = F[0 * 3 + 1];
    double f13 = F[0 * 3 + 2];
    double f21 = F[1 * 3 + 0];
    double f22 = F[1 * 3 + 1];
    double f23 = F[1 * 3 + 2];
    double f31 = F[2 * 3 + 0];
    double f32 = F[2 * 3 + 1];
    double f33 = F[2 * 3 + 2];

    double detF = f11 * (f22 * f33 - f23 * f32) - f12 * (f21 * f33 - f23 * f31)
                + f13 * (f21 * f32 - f22 * f31);

    double detinv = 1.0 / detF;

    auto F_inv =
        std::array<double, 9>{ detinv * (f22 * f33 - f23 * f32), detinv * (-f12 * f33 + f13 * f32),
                               detinv * (f12 * f23 - f13 * f22), detinv * (-f21 * f33 + f23 * f31),
                               detinv * (f11 * f33 - f13 * f31), detinv * (-f11 * f23 + f13 * f21),
                               detinv * (f21 * f32 - f22 * f31), detinv * (-f11 * f32 + f12 * f31),
                               detinv * (f11 * f22 - f12 * f21) };

    // precompute useful quantities
    // J^2 - 1
    double Jsq_minus_1 = detF * detF - 1.;
    // log(J)
    double logJ = std::log(detF);

    // tr(C) - Cauchy-Green
    // double trC = 0.0;
    // for (int i = 0; i < 3; i++) {
    //     for (int J = 0; J < 3; J++) {
    //         trC += F[i * 3 + J] * F[i * 3 + J];
    //     }
    // }

    // this is equivalent to the above commented out for-loop
    double trC = F[0] * F[0] + F[1] * F[1] + F[2] * F[2] + F[3] * F[3] + F[4] * F[4] + F[5] * F[5]
               + F[6] * F[6] + F[7] * F[7] + F[8] * F[8];

    // (J^2 -1)/2 - log(J)
    double A = 0.5 * Jsq_minus_1 - logJ;
    // Jm - trC + D /*dim*/
    double C = (Jm - trC + 3 /*dim*/);
    // Jm / (Jm - trC + D /*dim*/))
    double B = Jm / C;

    // the Piola stress tensor
    auto P = std::array<double, 9>{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    // // compute the Piola stress tensor
    // for (int i = 0; i < 3; i++) {
    //     for (int J = 0; J < 3; J++) {
    //         P[i * 3 + J] = mu * B * F[i * 3 + J]
    //                      + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[J * 3 + i];
    //     }
    // }

    // this is equivalent to the above commented out for-loop
    P[0] = mu * B * F[0] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[0];
    P[1] = mu * B * F[1] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[3];
    P[2] = mu * B * F[2] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[6];
    P[3] = mu * B * F[3] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[1];
    P[4] = mu * B * F[4] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[4];
    P[5] = mu * B * F[5] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[7];
    P[6] = mu * B * F[6] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[2];
    P[7] = mu * B * F[7] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[5];
    P[8] = mu * B * F[8] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv[8];

    // all done
    return P;
}

auto
gent_mito(const auto & epsilon)
{
    // mito gent material
    auto material = mito::materials::gent(rho, kappa, mu, Jm);

    auto F = mito::matrix_t<3>{ 1.0 + epsilon, epsilon, epsilon, epsilon,      1.0 + epsilon,
                                epsilon,       epsilon, epsilon, 1.0 + epsilon };


    // all done
    return material.stress(F);
}


static void
GentArray(benchmark::State & state)
{
    // generate a random number between -0.01 and 0.01
    double epsilon = 0.01 * ((std::rand() % 20001 / 10000.0) - 1.0);

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(gent_array(epsilon));
    }
}

static void
GentMito(benchmark::State & state)
{
    // generate a random number between -0.01 and 0.01
    double epsilon = 0.01 * ((std::rand() % 20001 / 10000.0) - 1.0);

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(gent_mito(epsilon));
    }
}


// run benchmark for Gent constitutive update (array)
BENCHMARK(GentArray);
// run benchmark for Gent constitutive update (mito)
BENCHMARK(GentMito);


// run all benchmarks
BENCHMARK_MAIN();


// end of file
