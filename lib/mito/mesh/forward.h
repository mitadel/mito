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

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = topology::oriented_simplex_t<D>;

    // element set alias
    template <class elementT>
    using element_set_t = topology::element_set_t<elementT>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = topology::oriented_simplex_ptr<D>;

    // point alias
    template <int D>
    using point_t = geometry::point_t<D>;
}


#endif

// end of file
