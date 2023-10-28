// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // available metrics
    enum metric_t { EUCLIDEAN, POLAR };

    // class manifold
    template <metric_t metricT, class cellT, int D>
    class Manifold;

    // class P-form
    template <int P, class F>
    class Form;

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
