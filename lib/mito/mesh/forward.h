// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <int D, template <int> class elementT>
    class Mesh;

    // mesh alias
    template <int D, template <int> class elementT>
    using mesh_t = Mesh<D, elementT>;

    // vertex alias
    using vertex_t = topology::vertex_t;

    // simplex alias
    template <int D>
    using simplex_t = topology::simplex_t<D>;

    // element set alias
    template <class elementT>
    using element_set_t = topology::element_set_t<elementT>;

    // topology alias
    using topology_t = topology::topology_t;

    // point alias
    template <int D>
    using point_t = geometry::point_t<D>;

    // mapping from vertices to points
    template <int D>
    using vertex_point_table_t =
        std::unordered_map<vertex_t, point_t<D>, mito::topology::element_hash<vertex_t>>;
}


#endif

// end of file
