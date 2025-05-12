// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // struct storing the type of an isoparametric simplex of order {order} and polynomial degree
    // {degree} on a geometric simplex of type {geometricSimplexT} and coordinate system
    // {coordinateSystemT}
    template <
        int order, int degree, geometry::geometric_simplex_c geometricSimplexT,
        geometry::coordinate_system_c coordinateSystemT>
    struct isoparametric_simplex {};

    // linear shape functions on triangles
    template <
        geometry::geometric_simplex_c geometricSimplexT,
        geometry::coordinate_system_c coordinateSystemT>
    struct isoparametric_simplex<2, 1, geometricSimplexT, coordinateSystemT> {
        using type = IsoparametricTriangle1<geometricSimplexT, coordinateSystemT>;
    };

}


// end of file
