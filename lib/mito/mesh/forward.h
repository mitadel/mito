// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <int D>
    class Mesh;

    // mesh alias
    template <int D>
    using mesh_t = Mesh<D>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = mito::topology::oriented_simplex_t<D>;

    // element set alias
    template <class elementT>
    using simplex_set_t = mito::topology::simplex_set_t<elementT>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = mito::topology::oriented_simplex_ptr<D>;

    // point alias
    template <int D>
    using point_t = mito::geometry::point_t<D>;
}


#endif

// end of file
