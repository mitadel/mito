// code guard
#if !defined(mito_quadrature_api_h)
#define mito_quadrature_api_h


namespace mito::quadrature {

    // integrator factory
    template <class quadratureT, int r, class elementSetT>
    constexpr auto integrator(const elementSetT & elementSet);

}


#endif

// end of file
