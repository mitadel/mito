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

    // point alias
    template <int D>
    using point_t = geometry::point_t<D>;
}


#endif

// end of file
