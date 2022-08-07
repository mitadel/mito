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
}


#endif

// end of file
