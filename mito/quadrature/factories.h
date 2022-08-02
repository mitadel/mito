// code guard
#if !defined(mito_quadrature_factories_h)
#define mito_quadrature_factories_h


namespace mito::quadrature {

    // integrator factory
    template <class quadratureT, int r, class manifoldT>
    constexpr auto integrator(const manifoldT & manifold)
    {
        return integrator_t<quadratureT, r, manifoldT>(manifold);
    }

}


#endif

// end of file
