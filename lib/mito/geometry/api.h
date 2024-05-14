// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_api_h)
#define mito_geometry_api_h


namespace mito::geometry {

    // metric space alias
    template <geometry::coordinates_c coordsT>
    using metric_space_t = MetricSpace<coordsT>;

    // coordinate system alias
    template <coordinates_c coordT>
    using coordinate_system_t = CoordinateSystem<coordT>;

    // point alias
    template <int D>
    using point_t = utilities::shared_ptr<const Point<D>>;

    // geometric simplex alias
    template <int N, int D>
    using geometric_simplex_t = GeometricSimplex<N, D>;

    // node alias
    template <int D>
    using node_t = geometric_simplex_t<0, D>;

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
    using edge_simplex_directors_t = std::array<vector_t<D>, N>;

    // point cloud factory
    template <int D>
    auto point_cloud() -> point_cloud_t<D> &;
}


#endif

// end of file
