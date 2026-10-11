// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials::thermal {

    // constant isotropic material factory
    template <int D>
    constexpr auto constant_isotropic(
        tensor::scalar_t kappa, tensor::scalar_t rho, tensor::scalar_t cp, tensor::scalar_t sigma)
        -> constant_isotropic_t<D>;

}


// end of file
