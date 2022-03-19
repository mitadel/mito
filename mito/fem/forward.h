// code guard
#if !defined(mito_fem_forward_h)
#define mito_fem_forward_h


namespace mito::fem {

    // class nodal field
    template <typename T, int D>
    class NodalField;

    // nodal field alias
    template <typename T, int D>
    using nodal_field_t = NodalField<T, D>;

    // class quadrature field
    template <int Q, class Y>
    class QuadratureField;

    // quadrature field alias
    template <int Q, class Y>
    using quadrature_field_t = QuadratureField<Q, Y>;
}


#endif

// end of file