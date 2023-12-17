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
        using GeometrySingleton = utilities::Singleton<Geometry<D>>;
        friend GeometrySingleton;
    };

}    // namespace mito

#endif    // mito_geometry_Geometry_h

// end of file
