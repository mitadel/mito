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

    // class geometry
    template <int D>
    class Geometry;

    // geometry alias
    template <int D>
    using geometry_t = Geometry<D>;

    // a cloud of points
    template <int D>
    using cloud_t = mito::utilities::segmented_t<typename point_t<D>::resource_t>;

    // class node
    template <int D>
    class Node;

    // a node is a pair of a vertex and a point
    template <int D>
    using node_t = Node<D>;
}


#endif

// end of file
