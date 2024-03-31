// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateType { CARTESIAN, POLAR };

    // class coordinates
    template <int D, CoordinateType coordT>
    class Coordinates;

    // class coordinate system
    template <int D, CoordinateType coordT>
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


#endif

// end of file
