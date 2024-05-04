// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito/base.h>
#include <mito/fields.h>
#include <pyre/timers.h>


// alias for cartesian coordinates
using mito::geometry::CARTESIAN;
// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, CARTESIAN>;


auto
random_number()
{
    // seed the random number generator
    std::srand(time(0));
    // a truly random_number
    auto rand_num = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    // a point in 2D space
    return mito::geometry::coordinates<coordinates_t>({ rand_num, rand_num });
}


void
laplacian_baseline(int N, const coordinates_t & x)
{
    // make a channel
    pyre::journal::info_t channel("benchmarks.laplacian");

    // make a timer
    pyre::timers::process_timer_t t("benchmarks.laplacian");

    // reset timer
    t.reset();
    // start timer
    t.start();

    // evaluate the laplacian product on {x}
    std::vector<double> result(N);
    for (int i = 0; i < N; ++i) {
        result[i] = 12 * x[0] * x[0] * x[1] * x[1] * x[1] * x[1]
                  + 12 * x[1] * x[1] * x[0] * x[0] * x[0] * x[0];
    }

    // stop the timer
    t.stop();

    // report
    channel << "baseline" << pyre::journal::newline << pyre::journal::indent(1)
            << "result = " << result[0] << pyre::journal::newline << "process time = " << t.ms()
            << " ms " << pyre::journal::newline << pyre::journal::outdent(1) << pyre::journal::endl;

    // all done
    return;
}


void
laplacian_mito(int N, const coordinates_t & x)
{
    // make a channel
    pyre::journal::info_t channel("benchmarks.laplacian");

    // make a timer
    pyre::timers::process_timer_t t("benchmarks.laplacian");

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

    // reset timer
    t.reset();
    // start timer
    t.start();

    // evaluate the laplacian product on {x}
    std::vector<double> result(N);
    for (int i = 0; i < N; ++i) {
        result[i] = laplacian(x);
    }

    // stop the timer
    t.stop();

    // report
    channel << "mito" << pyre::journal::newline << pyre::journal::indent(1)
            << "result = " << result[0] << pyre::journal::newline << "process time = " << t.ms()
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
    pyre::journal::info_t channel("tests.timer.laplacian");

    // report
    channel << "Computing " << N << " evaluations of laplacian" << pyre::journal::endl;

    // generate a truly random set of coordinates
    auto x = random_number();

    // compute {N} evaluations of laplacian without mito
    laplacian_baseline(N, x);

    // compute {N} evaluations of laplacian with mito
    laplacian_mito(N, x);

    // all done
    return 0;
}