// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // specialization for linear shape functions on segments embedded in 2D
    template <>
    struct isoparametric_simplex<1, geometry::segment_t<2>> {
        using type = IsoparametricEmbeddedSegmentP1;
    };

}


// end of file
