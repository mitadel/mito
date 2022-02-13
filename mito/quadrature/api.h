// code guard
#if !defined(mito_quadrature_api_h)
#define mito_quadrature_api_h


namespace mito::quadrature {

    // integrator alias
    template <class quadratureT, int r, class elementSetT>
    using integrator_t = Integrator<quadratureT, r, elementSetT>;

    // integrator factory
    template <class quadratureT, int r, class elementSetT>
    constexpr auto integrator(const elementSetT & elementSet)
    {
        return integrator_t<quadratureT, r, elementSetT>(elementSet);
    }
}


#endif

// end of file
