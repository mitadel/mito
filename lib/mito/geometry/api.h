// code guard
#if !defined(mito_geometry_api_h)
#define mito_geometry_api_h


namespace mito::geometry {

    // coordinates alias in D-dimensions
    template <int D, CoordinateType coordT>
    using coordinates_t = Coordinates<D, coordT>;

    // coordinates alias in D-dimensions
    template <int D, CoordinateType coordT>
    using coordinate_system_t = CoordinateSystem<D, coordT>;

    // point alias
    template <int D>
    using point_t = utilities::shared_ptr<const Point<D>>;

    // geometric simplex alias
    template <int N, int D>
    using geometric_simplex_t = utilities::shared_ptr<const GeometricSimplex<N, D>>;

    // point cloud alias
    template <int D>
    using point_cloud_t = PointCloud<D>;

    // geometry alias
    template <int D>
    using geometry_t = Geometry<D>;

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
