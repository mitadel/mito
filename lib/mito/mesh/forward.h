// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <class cellT, int D>
    class Mesh;

    // mesh alias
    template <class cellT, int D>
    using mesh_t = Mesh<cellT, D>;

    // vertex alias
    using vertex_t = topology::vertex_t;

    // element set alias
    template <class cellT>
    using element_set_t = topology::element_set_t<cellT>;

    // topology alias
    using topology_t = topology::topology_t;

    // point cloud alias
    template <int D>
    using point_cloud_t = geometry::point_cloud_t<D>;

    // point alias
    template <int D>
    using point_t = geometry::point_t<D>;

    template <int D>
    using node_t = geometry::node_t<D>;
}


#endif

// end of file
