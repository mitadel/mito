// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // specialization for linear shape functions on segments in 1D
    template <>
    struct isoparametric_simplex<1, geometry::segment_t<1>> {
        using type = IsoparametricSegmentP1;
    };

}


// end of file
