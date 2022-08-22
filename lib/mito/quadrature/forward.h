// code guard
#if !defined(mito_quadrature_forward_h)
#define mito_quadrature_forward_h


namespace mito::quadrature {

    // class integrator
    template <class quadratureT, int r, class elementSetT>
    class Integrator;

    // integrator alias
    template <class quadratureT, int r, class elementSetT>
    using integrator_t = Integrator<quadratureT, r, elementSetT>;

}


#endif

// end of file
