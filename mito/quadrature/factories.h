// code guard
#if !defined(mito_quadrature_factories_h)
#define mito_quadrature_factories_h


namespace mito::quadrature {

    // integrator factory
    template <class quadratureT, int r, class elementSetT>
    constexpr auto integrator(const elementSetT & elementSet)
    {
        return integrator_t<quadratureT, r, elementSetT>(elementSet);
    }

}


#endif

// end of file
