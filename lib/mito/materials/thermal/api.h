// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials::thermal {

    // constant isotropic material factory
    constexpr auto constant_isotropic(real rho, real E, real nu, real sigma) -> constant_isotropic_t;

}


// end of file
