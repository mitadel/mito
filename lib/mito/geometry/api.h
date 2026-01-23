// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // coordinate system alias
    template <coordinates_c coordT>
    using coordinate_system_t = CoordinateSystem<coordT>;

    // point alias
    template <int D>
    using point_t = utilities::shared_ptr<const Point<D>>;

    // reference simplex alias
    template <int N>
    using reference_simplex_t = ReferenceSimplex<N>;

    // geometric simplex alias
    template <int N, int D>
    using geometric_simplex_t = GeometricSimplex<N, D>;

    // node alias
    template <int D>
    using node_t = utilities::std_shared_ptr<const geometric_simplex_t<0, D>>;

    // reference segment alias
    using reference_segment_t = reference_simplex_t<1>;

    // reference triangle alias
    using reference_triangle_t = reference_simplex_t<2>;

    // reference tetrahedron alias
    using reference_tetrahedron_t = reference_simplex_t<3>;

    // segment alias
    template <int D>
    requires(D > 0)
    using segment_t = geometric_simplex_t<1, D>;

    // triangle alias
    template <int D>
    requires(D > 1)
    using triangle_t = geometric_simplex_t<2, D>;

    // tetrahedron alias
    template <int D>
    requires(D > 2)
    using tetrahedron_t = geometric_simplex_t<3, D>;

    // point cloud alias
    template <int D>
    using point_cloud_t = PointCloud<D>;

    // alias for the collection of oriented simplex director vectors
    // (e.g. an N-simplex has as N director vectors)
    template <int N, int D>
    using edge_simplex_directors_t = std::array<tensor::vector_t<D>, N>;

    // point cloud factory
    template <int D>
    auto point_cloud() -> point_cloud_t<D> &;
}


// end of file
