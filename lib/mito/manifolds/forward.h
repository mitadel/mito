// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class cellT, int D>
    class Manifold;

    // class one-form
    template <class F>
    class OneForm;

    // class tensor
    template <class F>
    class Tensor;

    // class field
    template <class F>
    class Field;

    // TOFIX: remove this datastructure
    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;
}


#endif

// end of file
