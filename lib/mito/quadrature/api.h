// code guard
#if !defined(mito_quadrature_api_h)
#define mito_quadrature_api_h


namespace mito::quadrature {

    // integrator factory
    template <class quadratureT, int r, class manifoldT>
    constexpr auto integrator(const manifoldT & manifold);
}


#endif

// end of file
