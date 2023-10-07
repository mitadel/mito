#include <mito/base.h>
#include <mito/manifolds.h>
#include <pyre/timers.h>


// type aliases
using process_timer_t = pyre::timers::process_timer_t;


void
wedge_array(int N)
{
    // make a channel
    pyre::journal::info_t channel("benchmarks.wedge");

    // make a timer
    pyre::timers::process_timer_t t("benchmarks.wedge");

    // a vector representing a one-form
    std::array<double, 3> a = { 2.0, 1.0, 0.0 };

    // a vector representing another one-form
    std::array<double, 3> b = { 0.0, 0.0, 1.0 };

    // a vector
    std::array<double, 3> xi1 = { 0.0, 0.0, 1.0 };

    // another vector
    std::array<double, 3> xi2 = { 0.0, 1.0, 0.0 };

    // reset timer
    t.reset();
    // start timer
    t.start();

    // evaluate the wedge product on {xi1} and {xi2}
    std::vector<double> result(N);
    for (int i = 0; i < N; ++i) {
        result[i] = (a[0] * xi1[0] + a[1] * xi1[1] + a[2] * xi1[2])
                      * (b[0] * xi2[0] + b[1] * xi2[1] + b[2] * xi2[2])
                  - (b[0] * xi1[0] + b[1] * xi1[1] + b[2] * xi1[2])
                        * (a[0] * xi2[0] + a[1] * xi2[1] + a[2] * xi2[2]);
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


void
wedge_mito(int N)
{
    // make a channel
    pyre::journal::info_t channel("benchmarks.wedge");

    // make a timer
    pyre::timers::process_timer_t t("benchmarks.wedge");

    // a vector
    auto a = 2.0 * mito::e_0<3> + mito::e_1<3>;

    // the corresponding one-form
    auto a_tilda = mito::manifolds::one_form(a);

    // another vector
    auto b = mito::e_2<3>;

    // the corresponding one-form
    auto b_tilda = mito::manifolds::one_form(b);

    // the wedge product of {a_tilda} and {b_tilda} (a two-form)
    auto ab_two_form = wedge(a_tilda, b_tilda);

    // a vector
    auto xi1 = mito::e_2<3>;

    // another vector
    auto xi2 = mito::e_1<3>;

    // reset timer
    t.reset();
    // start timer
    t.start();

    // evaluate the wedge product on {xi1} and {xi2}
    std::vector<double> result(N);
    for (int i = 0; i < N; ++i) {
        result[i] = ab_two_form(xi1)(xi2);
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
    pyre::journal::info_t channel("tests.timer.gent");

    // report
    channel << "Computing " << N << " wedge products of one forms" << pyre::journal::endl;

    // compute {N} wedge products with {std::array}
    wedge_array(N);

    // compute {N} wedge products with mito datastructures
    wedge_mito(N);

    // all done
    return 0;
}