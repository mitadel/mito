// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials::thermal {


    // constant isotropic material
    template <int D>
    class ConstantIsotropic;

    // constant isotropic material alias
    template <int D>
    using constant_isotropic_t = ConstantIsotropic<D>;
}


// end of file
