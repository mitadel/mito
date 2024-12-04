// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    // linear_elastic material factory
    constexpr auto linear_elastic(real rho, real E, real nu) -> linear_elastic_t
    {
        return linear_elastic_t(rho, E, nu);
    }

    // gent material factory
    constexpr auto gent(real rho, real kappa, real mu, real Jm) -> gent_t
    {
        return gent_t(rho, kappa, mu, Jm);
    }
}


// end of file
