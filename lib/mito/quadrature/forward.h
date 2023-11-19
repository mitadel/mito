// code guard
#if !defined(mito_quadrature_forward_h)
#define mito_quadrature_forward_h


namespace mito::quadrature {

    // available quadrature formulae
    enum quadrature_formula { GAUSS };

    // class integrator
    template <quadrature_formula quadratureT, int r, class manifoldT>
    class Integrator;

    // class quadrature rule
    template <quadrature_formula quadratureT, class elementT, int r>
    class QuadratureRule;
}


#endif

// end of file
