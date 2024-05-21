// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // class metric space
    template <coordinates_c coordsT>
    class MetricSpace;

    // class coordinate system
    template <coordinates_c coordT>
    class CoordinateSystem;

    // class point
    template <int D>
    class Point;

    // class geometric simplex
    template <int N, int D>
    requires((N >= 0) && (N <= D) && (D > 0))
    class GeometricSimplex;

    // class point cloud
    template <int D>
    class PointCloud;
}


// end of file
