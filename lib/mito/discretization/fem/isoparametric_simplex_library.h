// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


#include "tri1/public.h"
#include "tri2/public.h"


namespace mito::discretization {

    // struct storing the type of an isoparametric simplex of polynomial degree {degree} on a
    // geometric simplex of type {geometricSimplexT}
    template <int degree, geometry::geometric_simplex_c geometricSimplexT>
    struct isoparametric_simplex {};

    // specialization for linear shape functions on triangles in 2D
    template <>
    struct isoparametric_simplex<1, geometry::triangle_t<2>> {
        using type = IsoparametricTriangleP1;
    };

    // specialization for quadratic shape functions on triangles in 2D
    template <>
    struct isoparametric_simplex<2, geometry::triangle_t<2>> {
        using type = IsoparametricTriangleP2;
    };
}


// end of file
