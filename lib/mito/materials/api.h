// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_materials_api_h)
#define mito_materials_api_h


namespace mito::materials {

    // gent material factory
    constexpr auto gent(real rho, real kappa, real mu, real Jm) -> gent_t;
}


#endif

// end of file
