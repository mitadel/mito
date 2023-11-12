// code guard
#if !defined(mito_quadrature_Integrator_h)
#define mito_quadrature_Integrator_h


namespace mito::quadrature {

    // template with respect to element type T and to degree of exactness r of quadrature rule
    template <class quadratureT, int r, class manifoldT>
    class Integrator {
        using quadrature_t = quadratureT;
        using manifold_t = manifoldT;
        using cell_type = typename manifold_t::cell_type;
        static constexpr int D = manifold_t::dim;

        // quadrature_t, cell_type, and r identify a specific quadrature rule
        using QuadratureRule = QuadratureRulesFactory<quadrature_t, cell_type, r>;

      private:
        // QUESTION: Who should be in charge of computing the coordinates of the quadrature points
        //           in the elements? The quadrature rule has the coordinates of the quadrature
        //           points on the canonical element, the elements have the coordinate of the
        //           vertices.
        auto _computeQuadPointCoordinates() -> void
        {
            // loop on elements
            int e = 0;
            for (const auto & element : _manifold.elements()) {
                // loop on quadrature point
                for (int q = 0; q < Q; ++q) {
                    // use manifold parametrization to map the position of quadrature points in
                    // the canonical element to the coordinate of the quadrature point
                    _coordinates[{ e, q }] =
                        _manifold.parametrization(element, _quadratureRule.getPoint(q));
                }
                ++e;
            }

            // all done
            return;
        }

      public:
        Integrator(const manifold_t & manifold) :
            _manifold(manifold),
            _coordinates(manifold.nElements())
        {
            _computeQuadPointCoordinates();
        }

        template <class Y>
        auto integrate(const math::field_t<vector_t<D>, Y> & field) const -> Y
        {
            auto result = Y();

            // assemble elementary contributions
            for (auto e = 0; e < _manifold.nElements(); ++e) {
                for (auto q = 0; q < Q; ++q) {
                    result += field(_coordinates[{ e, q }]) * _quadratureRule.getWeight(q)
                            * _manifold.jacobian(e);
                }
            }

            return result;
        }

      private:
        // the quadrature rule
        static constexpr auto _quadratureRule = QuadratureRule::Get();
        // the number of quadrature points
        static constexpr int Q = std::size(_quadratureRule);
        // the domain of integration
        const manifold_t & _manifold;
        // the coordinates of the quadrature points in the domain of integration
        fem::quadrature_field_t<Q, vector_t<D>> _coordinates;
    };

}    // namespace  mito

#endif    // mito_quadrature_Integrator_h

// end of file
