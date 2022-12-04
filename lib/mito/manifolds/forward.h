// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <int I, int D, template <int> class elementT>
    requires(I <= D)
    class Manifold;

    // manifold alias
    template <int I, int D, template <int> class elementT>
    requires(I <= D)
    using manifold_t = Manifold<I, D, elementT>;

    // TOFIX: remove this datastructure
    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;
}


#endif

// end of file
