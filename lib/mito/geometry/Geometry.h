// code guard
#if !defined(mito_geometry_Geometry_h)
#define mito_geometry_Geometry_h


namespace mito::geometry {

    template <int D>
    class Geometry {
      public:
        // typedef for a topology
        using topology_t = topology::topology_t;
        // typedef for a vertex
        using vertex_t = topology::vertex_t;

      private:
        // constructor
        Geometry(topology_t & topology, point_cloud_t<D> & point_cloud) :
            _nodes(),
            _topology(topology),
            _point_cloud(point_cloud)
        {}

        // destructor
        ~Geometry() {}

      public:
        // register {vertex}-{point} relation as a new node
        inline auto node(const vertex_t & vertex, const point_t<D> & point) -> void
        {
            // register the node with the geometry
            _nodes.emplace(node_t<D>(vertex, point));

            // all done
            return;
        }

        // instantiate a new vertex and a new point at {coord} and bind them into a node
        inline auto node(vector_t<D> && coord) -> const vertex_t &
        {
            // ask the topology for a new vertex
            auto & vertex = _topology.vertex();

            // ask the point cloud for a point with coordinates {coord}
            auto & point = _point_cloud.point(std::move(coord));

            // register the node with the geometry
            _nodes.emplace(node_t<D>(vertex, point));

            // return a reference to the newly emplaced node
            return vertex;
        }

        // accessor to the collection of nodes
        inline auto nodes() const -> const nodes_t<D> & { return _nodes; }

        // get the point in space associated to this vertex
        inline auto point(const vertex_t & vertex) const -> const point_t<D> &
        {
            return _nodes.find(vertex)->second;
        }

        // accessor for topology
        inline auto topology() -> topology_t & { return _topology; }

        // const accessor for topology
        inline auto topology() const -> const topology_t & { return _topology; }

        // accessor for point cloud
        inline auto point_cloud() -> point_cloud_t<D> & { return _point_cloud; }

        // const accessor for point cloud
        inline auto point_cloud() const -> const point_cloud_t<D> & { return _point_cloud; }

      private:
        // the collection of nodes
        nodes_t<D> _nodes;

        // a reference to the topology
        topology_t & _topology;

        // a reference to the point cloud
        point_cloud_t<D> & _point_cloud;

        // friendship with the singleton
        using GeometrySingleton = utilities::Singleton<Geometry<D>>;
        friend GeometrySingleton;
    };

}    // namespace mito

#endif    // mito_geometry_Geometry_h

// end of file
