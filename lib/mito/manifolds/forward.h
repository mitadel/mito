// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class cellT, int D>
    class Manifold;

    // manifold alias
    template <class cellT, int D>
    using manifold_t = Manifold<cellT, D>;

    // TOFIX: remove this datastructure
    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;
}


#endif

// end of file
