// code guard
#if !defined(mito_geometry_Geometry_h)
#define mito_geometry_Geometry_h


namespace mito::geometry {

    template <int D, CoordinateSystem coordT>
    class Geometry {
      private:
        // typedef for a topology
        using topology_type = topology::topology_t;
        // typedef for a vertex
        using vertex_type = topology::vertex_t;
        // typedef for a point
        using point_type = point_t<D, coordT>;
        // typedef for a point cloud
        using point_cloud_type = point_cloud_t<D, coordT>;
        // typedef for a nodes collection
        using nodes_type = nodes_t<D, coordT>;
        // typedef for a node
        using node_type = node_t<D, coordT>;
        // typedef for a set of coordinates
        using coordinates_type = coordinates_t<D, coordT>;

      private:
        // constructor
        Geometry(topology_type & topology, point_cloud_type & point_cloud) :
            _nodes(),
            _topology(topology),
            _point_cloud(point_cloud)
        {}

        // destructor
        ~Geometry() {}

      public:
        // register {vertex}-{point} relation as a new node
        inline auto node(const vertex_type & vertex, const point_type & point) -> void
        {
            // register the node with the geometry
            _nodes.emplace(node_type(vertex, point));

            // all done
            return;
        }

        // instantiate a new vertex and a new point at {coord} and bind them into a node
        inline auto node(coordinates_type && coord) -> vertex_type
        {
            // ask the topology for a new vertex
            auto vertex = _topology.vertex();

            // ask the point cloud for a point with coordinates {coord}
            auto point = _point_cloud.point(std::move(coord));

            // register the node with the geometry
            _nodes.emplace(node_type(vertex, point));

            // return a reference to the newly emplaced node
            return vertex;
        }

        // accessor to the collection of nodes
        inline auto nodes() const noexcept -> const nodes_type & { return _nodes; }

        // get the point in space associated to this vertex
        inline auto point(const vertex_type & vertex) const -> point_type
        {
            return _nodes.find(vertex)->second;
        }

        // construct the vector that goes from the first to the second end of this segment
        inline auto vector(const topology::segment_t & segment) const -> vector_t<D>
        {
            // get the segment composition
            auto composition = segment->composition();

            // get the coordinates of the first end of the segment
            auto point0 = point(composition[0]->footprint())->coordinates();

            // get the coordinates of the second end of the segment
            auto point1 = point(composition[1]->footprint())->coordinates();

            // construct the vector from {point0} to {point1} and return it
            return point1 - point0;
        }

        // TOFIX: split this header into interface and implementation
      private:
        template <int N, int... J>
        inline auto _directors(const topology::simplex_t<N> & simplex, integer_sequence<J...>) const
            -> std::pair<coordinates_type, topology::edge_simplex_directors_t<N, D>>
        requires(sizeof...(J) == N)
        {
            // get the simplex vertices
            auto vertices = simplex->vertices();

            // get the coordinates of the first vertex
            const auto & p0 = point(vertices[0])->coordinates();

            // TOFIX: the distance of two points must be a vector
            // TOFIX: implement a coordinate system as a basis for the space + an origin
            // compute the director vectors associated with each director edge
            // auto directors = std::array { _mesh.geometry().vector(edge_directors[J])... };
            auto directors = topology::edge_simplex_directors_t<N, D> { (
                point(vertices[J + 1])->coordinates() - p0)... };

            // all done
            return { p0, directors };
        }

      public:
        // return the array of director vectors of the simplex and the coordinates of the point
        // where they stem from
        template <int N>
        inline auto directors(const topology::simplex_t<N> & simplex) const
            -> std::pair<coordinates_type, topology::edge_simplex_directors_t<N, D>>
        {
            return _directors(simplex, make_integer_sequence<N> {});
        }

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

      private:
        // the collection of nodes
        nodes_type _nodes;

        // a reference to the topology
        topology_type & _topology;

        // a reference to the point cloud
        point_cloud_type & _point_cloud;

        // friendship with the singleton
        using GeometrySingleton = utilities::Singleton<Geometry<D, coordT>>;
        friend GeometrySingleton;
    };

}    // namespace mito

#endif    // mito_geometry_Geometry_h

// end of file
