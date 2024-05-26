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

    // concept of a coordinate system
    template <class F>
    concept coordinate_system_c = requires(F c) {
        // require that F only binds to {CoordinateSystem} specializations
        []<coordinates_c coordT>(const CoordinateSystem<coordT> &) {
        }(c);
    };

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

    // concept of a point cloud
    template <class F>
    concept point_cloud_c = requires(F c) {
        // require that F only binds to {PointCloud} specializations
        []<int D>(const PointCloud<D> &) {
        }(c);
    };
}


// end of file
