// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // specialization for quadratic shape functions on triangles in 2D
    template <>
    struct isoparametric_simplex<2, geometry::triangle_t<2>> {
        using type = IsoparametricTriangleP2;
    };

}


// end of file
