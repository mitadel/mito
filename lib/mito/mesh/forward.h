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

    // mapping from vertices to points
    template <int D>
    using vertex_point_table_t =
        std::unordered_map<vertex_t, point_t<D>, mito::topology::cell_hash<vertex_t>>;
}


#endif

// end of file
