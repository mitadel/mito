// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // gamma of Alexander's SDIRK3 scheme: the root of x^3 - 3x^2 + 3/2 x - 1/6 = 0 in
    // (1/6, 1/2), computed at compile time with Newton's method (gamma ~ 0.4358665215)
    consteval auto alexander_gamma() -> double
    {
        double x = 0.43;
        for (int i = 0; i < 20; ++i) {
            const double p = ((x - 3.) * x + 1.5) * x - 1. / 6.;
            const double dp = (3. * x - 6.) * x + 1.5;
            x -= p / dp;
        }
        return x;
    }

    // the rate of a stage solved with shift {alpha} and predictor {beta}: U_dot = alpha (U - beta)
    template <class vectorT, class scalarT>
    auto stage_rate(scalarT alpha, const vectorT & beta, const vectorT & U, vectorT & U_dot) -> void
    {
        // U_dot = U
        U_dot.copy_values(U);

        // U_dot = U - beta
        U_dot.add(-1.0, beta);

        // U_dot = alpha (U - beta)
        U_dot.scale(alpha);

        // all done
        return;
    }
}


// end of file
