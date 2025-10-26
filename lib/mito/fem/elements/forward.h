// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // the struct that implements the discretization strategy
    template <class elementT, discretization_t discretizationT>
    struct Discretizer;

    // struct storing the type of an isoparametric simplex of polynomial degree {degree} on a
    // geometric simplex of type {geometricSimplexT}
    template <int degree, geometry::geometric_simplex_c geometricSimplexT>
    struct isoparametric_simplex;

}


// end of file
