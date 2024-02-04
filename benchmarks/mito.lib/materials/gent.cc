// get array
#include <array>
#include <cstdlib>

// get support
#include <mito/base.h>
#include <mito/materials.h>
#include <pyre/timers.h>


// type aliases
using process_timer_t = pyre::timers::process_timer_t;


void
gent_constitutive_array(
    int N, double kappa, double mu, double Jm, double epsilon,
    std::array<double, 9> & P_array_result)
{
    // make a channel
    pyre::journal::info_t channel("tests.timer.gent");

    // make a timer
    process_timer_t t("tests.timer");


    // ARRAY

    // array tensor
    std::array<double, 9> F_array{ 1.0 + epsilon, epsilon, epsilon, epsilon,      1.0 + epsilon,
                                   epsilon,       epsilon, epsilon, 1.0 + epsilon };

    // reset timer
    t.reset();
    // start timer
    t.start();

    // perform calculation N times
    for (int n = 0; n < N; ++n) {

        double f11 = F_array[0 * 3 + 0];
        double f12 = F_array[0 * 3 + 1];
        double f13 = F_array[0 * 3 + 2];
        double f21 = F_array[1 * 3 + 0];
        double f22 = F_array[1 * 3 + 1];
        double f23 = F_array[1 * 3 + 2];
        double f31 = F_array[2 * 3 + 0];
        double f32 = F_array[2 * 3 + 1];
        double f33 = F_array[2 * 3 + 2];

        double detF = f11 * (f22 * f33 - f23 * f32) - f12 * (f21 * f33 - f23 * f31)
                    + f13 * (f21 * f32 - f22 * f31);

        double detinv = 1.0 / detF;

        std::array<double, 9> F_inv_array{
            detinv * (f22 * f33 - f23 * f32), detinv * (-f12 * f33 + f13 * f32),
            detinv * (f12 * f23 - f13 * f22), detinv * (-f21 * f33 + f23 * f31),
            detinv * (f11 * f33 - f13 * f31), detinv * (-f11 * f23 + f13 * f21),
            detinv * (f21 * f32 - f22 * f31), detinv * (-f11 * f32 + f12 * f31),
            detinv * (f11 * f22 - f12 * f21)
        };

        // precompute useful quantities
        // J^2 - 1
        double Jsq_minus_1 = detF * detF - 1.;
        // log(J)
        double logJ = std::log(detF);

        // tr(C) - Cauchy-Green
        // double trC = 0.0;
        // for (int i = 0; i < 3; i++) {
        //     for (int J = 0; J < 3; J++) {
        //         trC += F_array[i * 3 + J] * F_array[i * 3 + J];
        //     }
        // }

        // this is equivalent to the above commented out for-loop
        double trC = F_array[0] * F_array[0] + F_array[1] * F_array[1] + F_array[2] * F_array[2]
                   + F_array[3] * F_array[3] + F_array[4] * F_array[4] + F_array[5] * F_array[5]
                   + F_array[6] * F_array[6] + F_array[7] * F_array[7] + F_array[8] * F_array[8];

        // (J^2 -1)/2 - log(J)
        double A = 0.5 * Jsq_minus_1 - logJ;
        // Jm - trC + D /*dim*/
        double C = (Jm - trC + 3 /*dim*/);
        // Jm / (Jm - trC + D /*dim*/))
        double B = Jm / C;

        // compute the Piola stress tensor
        // std::array<double, 9> P_array { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        // for (int i = 0; i < 3; i++) {
        //     for (int J = 0; J < 3; J++) {
        //         P_array[i * 3 + J] =
        //             mu * B * F_array[i * 3 + J]
        //             + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[J * 3 + i];
        //     }
        // }

        // accumulate result
        // for (int i = 0; i < 3; i++) {
        //     for (int J = 0; J < 3; J++) {
        //         P_array_result[i * 3 + J] += P_array[i * 3 + J];
        //     }
        // }

        // this is equivalent to the above commented out for-loops
        P_array_result[0] +=
            mu * B * F_array[0] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[0];
        P_array_result[1] +=
            mu * B * F_array[1] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[3];
        P_array_result[2] +=
            mu * B * F_array[2] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[6];
        P_array_result[3] +=
            mu * B * F_array[3] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[1];
        P_array_result[4] +=
            mu * B * F_array[4] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[4];
        P_array_result[5] +=
            mu * B * F_array[5] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[7];
        P_array_result[6] +=
            mu * B * F_array[6] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[2];
        P_array_result[7] +=
            mu * B * F_array[7] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[5];
        P_array_result[8] +=
            mu * B * F_array[8] + (2. * kappa * A * A * A * Jsq_minus_1 - mu) * F_inv_array[8];
    }

    // stop the timer
    t.stop();

    // report
    channel << "array (for loop)" << pyre::journal::newline << pyre::journal::indent(1)
            << "result = " << P_array_result[0] << ", " << P_array_result[1] << ", "
            << P_array_result[2] << ", " << P_array_result[3] << ", " << P_array_result[4] << ", "
            << P_array_result[5] << ", " << P_array_result[6] << ", " << P_array_result[7] << ", "
            << P_array_result[8] << pyre::journal::newline << "process time = " << t.ms() << " ms "
            << pyre::journal::newline << pyre::journal::outdent(1) << pyre::journal::endl;

    // all done
    return;
}


auto
gent_constitutive_mito(
    int N, double rho, double kappa, double mu, double Jm, double epsilon,
    mito::matrix_t<3> & P_result)
{
    // make a channel
    pyre::journal::info_t channel("tests.timer.gent");

    // make a timer
    process_timer_t t("tests.timer");

    // MITO

    // mito gent material
    auto material = mito::materials::gent(rho, kappa, mu, Jm);

    mito::vector_t<3> u{ 0.0, 0.0, 0.0 };
    mito::matrix_t<3> F{ 1.0 + epsilon, epsilon, epsilon, epsilon,      1.0 + epsilon,
                         epsilon,       epsilon, epsilon, 1.0 + epsilon };
    mito::matrix_t<3> P;

    // reset timer
    t.reset();
    // start timer
    t.start();

    // perform calculation N times
    for (int n = 0; n < N; ++n) {
        // evaluate constitutive update
        material.Constitutive(u, F, P);
        // accumulate result
        P_result += P;
    }

    // stop the timer
    t.stop();

    // report
    channel << "tensor" << pyre::journal::newline << pyre::journal::indent(1)
            << "result = " << P_result << pyre::journal::newline << "process time = " << t.ms()
            << " ms " << pyre::journal::newline << pyre::journal::outdent(1) << pyre::journal::endl;

    // all done
    return;
}


int
main()
{

    // number of times to do operation
    int N = 1 << 25;

    // make a channel
    pyre::journal::info_t channel("tests.timer.gent");

    channel << "Computing " << N << " gent constitutive updates" << pyre::journal::endl;

    // material parameters
    double rho = 1.0;
    double kappa = 1.0;
    double mu = 1.0;
    double Jm = 1.0;

    // Generate a random number between -0.01 and 0.01
    double epsilon = 0.01 * ((std::rand() % 20001 / 10000.0) - 1.0);

    // gent constitutive update
    std::array<double, 9> P_array_result{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    gent_constitutive_array(N, kappa, mu, Jm, epsilon, P_array_result);

    // gent constitutive update
    mito::matrix_t<3> P_result;
    gent_constitutive_mito(N, rho, kappa, mu, Jm, epsilon, P_result);

    // all done
    return 0;
}