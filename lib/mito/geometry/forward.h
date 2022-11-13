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

    template <int D>
    using cloud_t = mito::utilities::segmented_t<typename point_t<D>::resource_t>;

}


#endif

// end of file
