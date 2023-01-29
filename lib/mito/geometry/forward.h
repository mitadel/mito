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

    // hash function for {nodeT}
    template <class nodeT>
    struct node_hash {
        size_t operator()(const nodeT & node) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<mito::topology::unoriented_simplex_id_t>(
                static_cast<mito::topology::vertex_t::handle_t>(node.vertex().handle()));
        }
    };

    // mapping from vertices to points
    // TOFIX: remove eventually. This typedef should not leak from {Geometry}
    // TOFIX: is this going to be an {unordered_multiset} when we do DG (i.e. when we have
    // different nodes for the same vertex and same point)
    template <int D>
    using nodes_t = std::unordered_set<node_t<D>, node_hash<node_t<D>>>;
}


#endif

// end of file
