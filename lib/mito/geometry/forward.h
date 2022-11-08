// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // class point
    template <int D>
    class Point;

    // point alias
    template <int D>
    using point_t = mito::utilities::shared_ptr<Point<D>>;

    // class point cloud
    template <int D>
    class PointCloud;

    // point cloud alias
    template <int D>
    using point_cloud_t = PointCloud<D>;

    // vertex set alias
    using vertex_set_t = topology::vertex_set_t;

    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;

    // vertex vector alias
    using vertex_vector_t = topology::vertex_vector_t;

    // simplex alias
    template <int D>
    using simplex_t = topology::simplex_t<D>;

    // vertex alias
    using vertex_t = topology::vertex_t;

    // segment alias
    using segment_t = topology::segment_t;

    // triangle alias
    using triangle_t = topology::triangle_t;

    // tetrahedron alias
    using tetrahedron_t = topology::tetrahedron_t;

    template <int D>
    using cloud_t =
        mito::utilities::segmented_t<typename point_t<D>::resource_t, 100 /* segment size */>;

    // this information belongs to the mesh
    // std::unordered_map<vertex_t, point_t<D>, mito::topology::element_hash<vertex_t>>;

}


#endif

// end of file
