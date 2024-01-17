// code guard
#if !defined(mito_geometry_Geometry_h)
#define mito_geometry_Geometry_h


namespace mito::geometry {

    template <int D>
    class Geometry {
      private:
        // typedef for a topology
        using topology_type = topology::topology_t;
        // typedef for a vertex
        using vertex_type = topology::vertex_t;
        // typedef for a point
        using point_type = point_t<D>;
        // typedef for a point cloud
        using point_cloud_type = point_cloud_t<D>;
        // typedef for a nodes collection
        using nodes_type = nodes_t<D>;
        // typedef for a node
        using node_type = node_t<D>;
        // typedef for a geometric simplex
        template <int N>
        using geometric_simplex_type = geometric_simplex_t<N, D>;
        // typedef for a repository of oriented simplices
        template <int N>
        using geometric_simplex_repository_type =
            utilities::repository_t<geometric_simplex_type<N>>;
        // a map between points and their coordinates
        // TOFIX: change name to {nodes_type}
        // TOFIX: remove the ugly {resource_type}
        template <int N>
        using vertex_point_map_type = geometric_simplex_type<N>::resource_type::nodes_type;

      private:
        // constructor
        Geometry(topology_type & topology, point_cloud_type & point_cloud) :
            _nodes(),
            _topology(topology),
            _point_cloud(point_cloud),
            _segments(100 /*segment size */),
            _triangles(100 /*segment size */),
            _tetrahedra(100 /*segment size */)
        {}

        // destructor
        ~Geometry() {}

      public:
        // register {vertex}-{point} relation as a new node
        inline auto node(const vertex_type & vertex, const point_type & point) -> node_type
        {
            // register the node with the geometry
            return node_type(vertex, point);
        }

        // instantiate a new vertex and a new point at {coord} and bind them into a node
        inline auto node(const point_type & point) -> node_type
        {
            // ask the topology for a new vertex
            auto vertex = _topology.vertex();

            // register the node with the geometry
            return node_type(vertex, point);
        }

        // accessor to the collection of nodes
        inline auto nodes() const noexcept -> const nodes_type & { return _nodes; }

        // get the point in space associated to this vertex
        inline auto point(const vertex_type & vertex) const -> point_type
        {
            return _nodes.find(vertex)->second;
        }

        // TOFIX: split this header into interface and implementation
      public:
        // TOFIX: not sure if I like these accessors
        // accessor for topology
        inline auto topology() noexcept -> topology_type & { return _topology; }

        // const accessor for topology
        inline auto topology() const noexcept -> const topology_type & { return _topology; }

        // accessor for point cloud
        inline auto point_cloud() noexcept -> point_cloud_type & { return _point_cloud; }

        // const accessor for point cloud
        inline auto point_cloud() const noexcept -> const point_cloud_type &
        {
            return _point_cloud;
        }

        // QUESTION: what spatial search needs to be done to guarantee the uniqueness of the
        // simplex?

        // build the segment connecting two nodes
        constexpr auto segment(vertex_point_map_type<1> && nodes) -> geometric_simplex_type<1>
        requires(D > 0)
        {
            // fetch vertices
            auto vertex_0 = nodes[0].first;
            auto vertex_1 = nodes[1].first;

            // instantiate a segment
            const auto & simplex = _topology.segment({ vertex_0, vertex_1 });

            // all done
            return _segments.emplace(simplex, nodes);
        }

        // build the triangle connecting these three nodes
        constexpr auto triangle(vertex_point_map_type<2> && nodes) -> geometric_simplex_type<2>
        requires(D > 1)
        {
            // TOFIX: not a big fan of the {first} and {second} business
            // fetch vertices
            auto vertex_0 = nodes[0].first;
            auto vertex_1 = nodes[1].first;
            auto vertex_2 = nodes[2].first;

            // TOFIX: reserve the words {segment}, {triangle}, {tetrahedron} for the geometric
            // entities
            const auto & simplex = _topology.triangle({ vertex_0, vertex_1, vertex_2 });

            // all done
            return _triangles.emplace(simplex, nodes);
        }

        // build the tetrahedron connecting these four nodes
        constexpr auto tetrahedron(vertex_point_map_type<3> && nodes) -> geometric_simplex_type<3>
        requires(D > 1)
        {
            // TOFIX: not a big fan of the {first} and {second} business
            // fetch vertices
            auto vertex_0 = nodes[0].first;
            auto vertex_1 = nodes[1].first;
            auto vertex_2 = nodes[2].first;
            auto vertex_3 = nodes[3].first;

            // TOFIX: reserve the words {segment}, {triangle}, {tetrahedron} for the geometric
            // entities
            const auto & simplex =
                _topology.tetrahedron({ vertex_0, vertex_1, vertex_2, vertex_3 });

            // all done
            return _tetrahedra.emplace(simplex, nodes);
        }

      private:
        // the collection of nodes
        // TOFIX: remove
        nodes_type _nodes;

        // a reference to the topology
        topology_type & _topology;

        // a reference to the point cloud
        // TOFIX: remove
        point_cloud_type & _point_cloud;

        // TOFIX: is there a way to activate these if D>... ?
        // repository of segments
        geometric_simplex_repository_type<1> _segments;

        // repository of triangles
        geometric_simplex_repository_type<2> _triangles;

        // repository of tetrahedra
        geometric_simplex_repository_type<3> _tetrahedra;

        // friendship with the singleton
        using GeometrySingleton = utilities::Singleton<Geometry<D>>;
        friend GeometrySingleton;
    };

}    // namespace mito

#endif    // mito_geometry_Geometry_h

// end of file
