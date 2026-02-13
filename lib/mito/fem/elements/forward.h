// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // the struct that implements the discretization strategy
    template <class elementT, discretization_t discretizationT>
    struct Discretizer;

    // struct storing the type of an isoparametric simplex of polynomial degree {degree}
    // on a manifold of type {ManifoldT}
    // the manifold type encapsulates the coordinate system, volume form, and cell type
    template <int degree, class ManifoldT>
    struct isoparametric_simplex;

}


// end of file
