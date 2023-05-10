// code guard
#if !defined(mito_quadrature_utilities_h)
#define mito_quadrature_utilities_h


namespace mito::quadrature {

    // volume
    template <class manifoldT>
    auto volume(const manifoldT & manifold) -> real
    {
        // the constant field equal to one
        auto one = mito::math::field(mito::math::one<mito::vector_t<manifoldT::dim>>());
        // an integrator on the manifold
        auto integrator = mito::quadrature::integrator<GAUSS, 1>(manifold);
        // integrate the constant unit function and return
        return integrator.integrate(one);
    }

}


#endif

// end of file