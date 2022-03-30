// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class element set
    template <class element_t, int D>
    class ElementSet;

    // element set alias
    template <class elementT, int D>
    using element_set_t = ElementSet<elementT, D>;

}


#endif

// end of file
