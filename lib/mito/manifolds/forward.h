// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class element_t, int D>
    class Manifold;

    // manifold alias
    template <class elementT, int D>
    using manifold_t = Manifold<elementT, D>;

    // element set alias
    template <class elementT>
    using element_set_t = topology::element_set_t<elementT>;

    // vertex set alias
    using vertex_set_t = topology::vertex_set_t;

    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = topology::oriented_simplex_ptr<D>;
}


#endif

// end of file
