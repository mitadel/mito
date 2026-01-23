// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
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

    // material consistency test factory
    template <class materialT>
    constexpr auto consistency_test(
        const materialT & material, const mito::real & perturbation, const mito::real & tolerance)
        -> material_consistency_test_t<materialT>
    {
        return material_consistency_test_t<materialT>(material, perturbation, tolerance);
    }
}


// end of file
