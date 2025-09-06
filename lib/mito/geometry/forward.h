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

    // reference simplex
    template <int N>
    class ReferenceSimplex;

    // class geometric simplex
    template <int N, int D>
    requires((N >= 0) && (N <= D) && (D > 0))
    class GeometricSimplex;

    // concept of a geometric simplex
    template <class F>
    concept geometric_simplex_c = requires(F c) {
        // require that F only binds to {GeometricSimplex} specializations
        []<int N, int D>(const GeometricSimplex<N, D> &) {
        }(c);
    };

    // concept of a geometric segment (geometric simplex with order 1)
    template <class F>
    concept geometric_segment_c = geometric_simplex_c<F> && F::order == 1;

    // concept of a geometric triangle (geometric simplex with order 2)
    template <class F>
    concept geometric_triangle_c = geometric_simplex_c<F> && F::order == 2;

    // concept of a geometric tetrahedron (geometric simplex with order 3)
    template <class F>
    concept geometric_tetrahedron_c = geometric_simplex_c<F> && F::order == 3;

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
