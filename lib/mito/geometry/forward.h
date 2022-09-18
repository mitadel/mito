// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // class point
    template <int D>
    class Point;

    // point alias
    template <int D>
    using point_t = vector_t<D>;    // Point<D>;

    // class point cloud
    template <int D>
    class PointCloud;

    // point cloud alias
    template <int D>
    using point_cloud = PointCloud<D>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = topology::oriented_simplex_ptr<D>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = topology::oriented_simplex_t<D>;

    // vertex set alias
    using vertex_set_t = topology::vertex_set_t;

    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;

    // vertex vector alias
    using vertex_vector_t = topology::vertex_vector_t;

    // segment alias
    using segment_t = topology::segment_t;

    // triangle alias
    using triangle_t = topology::triangle_t;

    // tetrahedron alias
    using tetrahedron_t = topology::tetrahedron_t;
}


#endif

// end of file
