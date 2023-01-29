// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <int D, template <int> class cellT, int N>
    requires(N <= D)
    class Mesh;

    // mesh alias
    template <int D, template <int> class cellT, int N>
    requires(N <= D)
    using mesh_t = Mesh<D, cellT, N>;

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
