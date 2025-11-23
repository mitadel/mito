// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // specialization for linear shape functions on triangles in 2D
    template <>
    struct isoparametric_simplex<1, geometry::triangle_t<2>> {
        using type = IsoparametricTriangleP1;
    };

}


// end of file
