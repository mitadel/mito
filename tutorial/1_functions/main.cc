// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// mito support
#include <mito.h>


// strip namespace
using namespace mito;


int
main()
{
    // make a channel
    journal::info_t channel("tutorial.functions");

    // report
    channel << "a mito application" << journal::endl;

    // pi sixths
    auto pi_sixth = std::numbers::pi / 6.0;

    // function sin
    auto sin = functions::sin;
    // function x^3
    auto pow3 = functions::pow<3>;

    // function composition sin(x^3)
    auto sin_pow3 = sin(pow3);

    // the derivative of sin(x^3)
    auto d_sin_pow3 = functions::derivative(sin_pow3);
    // function cos
    auto cos = functions::cos;
    // function power of two
    auto pow2 = functions::pow<2>;

    // report result
    channel << "d(sin(x^3))/dx = " << d_sin_pow3(pi_sixth) << journal::endl;
    channel << "3*cos(x^3)*x^2 = " << 3.0 * cos(pow3(pi_sixth)) * pow2(pi_sixth) << journal::endl;

    // all done
    return 0;
}