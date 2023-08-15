// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // class point
    template <int D>
    class Point;

    // point alias
    template <int D>
    using point_t = utilities::shared_ptr<Point<D>>;

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

    // TOFIX: do we need this typedef here?
    // a cloud of points
    template <int D>
    using cloud_t = utilities::repository_t<point_t<D>>;

    // class node
    template <int D>
    class Node;

    // a node is a pair of a vertex and a point
    // template <int D>
    // using node_t = Node<D>;
    template <int D>
    using node_t = std::pair<topology::vertex_t, point_t<D>>;

    // mapping from vertices to points
    // TOFIX: is this going to be an {unordered_multimap} when we do DG (i.e. when we have
    // different nodes for the same vertex and same point)
    template <int D>
    using nodes_t = std::unordered_map<
        topology::vertex_t, point_t<D>, utilities::hash_function<topology::vertex_t>>;
}


#endif

// end of file
