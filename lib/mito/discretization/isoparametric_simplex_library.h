// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // struct storing the type of an isoparametric simplex of polynomial degree {degree} on a
    // geometric simplex of type {geometricSimplexT} and coordinate system {coordinateSystemT}
    template <
        int degree, geometry::geometric_simplex_c geometricSimplexT,
        geometry::coordinate_system_c coordinateSystemT>
    // require that the geometric simplex spatial dimension matches that of the coordinate system
    requires(coordinateSystemT::dim == geometricSimplexT::dim)
    struct isoparametric_simplex {};

    // specialization for linear shape functions on triangles
    template <geometry::coordinate_system_c coordinateSystemT>
    struct isoparametric_simplex<
        1, geometry::triangle_t<coordinateSystemT::dim>, coordinateSystemT> {
        using type =
            IsoparametricTriangle1<geometry::triangle_t<coordinateSystemT::dim>, coordinateSystemT>;
    };
}


// end of file
