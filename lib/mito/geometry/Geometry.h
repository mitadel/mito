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
        inline auto node(const vertex_t & vertex, const point_t<D> & point) -> const node_t<D> &
        {
            // register the node with the geometry
            auto ret = _nodes.emplace(node_t<D>(vertex, point));

            // return a reference to the newly emplaced node
            return *ret.first;
        }

        inline auto node(vector_t<D> && coord) -> auto
        {
            // ask the topology for a new vertex
            auto & vertex = _topology.vertex();

            // ask the point cloud for a point with coordinates {coord}
            auto & point = _point_cloud.point(coord);

            // return a reference to the newly emplaced node
            return node(vertex, point);
        }

      private:
        // the collection of nodes
        nodes_t<D> _nodes;

        // a reference to the topology
        topology_t & _topology;

        // a reference to the point cloud
        // QUESTION: should this point cloud be owned by {Geometry}?
        point_cloud_t<D> & _point_cloud;

        // friendship with the singleton
        using GeometrySingleton = mito::utilities::Singleton<Geometry>;
        friend GeometrySingleton;
    };

}    // namespace mito

#endif    // mito_geometry_Geometry_h

// end of file
