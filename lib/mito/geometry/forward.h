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
    using oriented_simplex_ptr = mito::topology::oriented_simplex_ptr<D>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = mito::topology::oriented_simplex_t<D>;
}


#endif

// end of file
