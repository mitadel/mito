// code guard
#if !defined(mito_quadrature_Integrator_h)
#define mito_quadrature_Integrator_h


namespace mito::quadrature {

    // TODO: Keep in mind that we will need integrator and the above defined fields to compute
    // integrals of contact forces down the road. Do we have enough machinery for that?

    // template with respect to element type T and to degree of exactness r of quadrature rule
    template <class quadratureT, int r, class manifoldT>
    class Integrator {
        using quadrature_t = quadratureT;
        using manifold_t = manifoldT;
        using element_t = typename manifold_t::element_t;
        static constexpr int D = manifold_t::dim;

        // quadrature_t, element_t, and r identify a specific quadrature rule
        using QuadratureRule = QuadratureRulesFactory<quadrature_t, element_t, r>;

      private:
        // QUESTION: Who should be in charge of computing the coordinates of the quadrature points
        //           in the elements? The quadrature rule has the coordinates of the quadrature
        //           points on the reference element, the elements have the coordinate of the
        //           vertices.
        void _computeQuadPointCoordinates()
        {
            // TOFIX: We should avoid the 4 nested for loops
            // QUESTION: 3 out 4 of these loops can be unrolled as Q and D are template parameters
            //           Is there anything we can do about it?

            // loop on elements
            int e = 0;
            for (const auto & element : _manifold.elements()) {
                // use a set to collect vertices without repeated entries
                mesh::vertex_set_t vertices;
                element->vertices(vertices);
                // loop on vertices
                int v = 0;
                for (const auto & vertex : vertices) {
                    // loop on quadrature point
                    for (int q = 0; q < Q; ++q) {
                        for (int d = 0; d < D; ++d) {
                            const auto & vertexCoordinates = _manifold.coordinatesVertex(vertex);
                            _coordinates[{ e, q }][d] +=
                                _quadratureRule.getPoint(q)[v] * vertexCoordinates[d];
                        }
                    }
                    ++v;
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
        Y integrate(const math::field_t<vector_t<D>, Y> & field) const
        {
            auto values = field(_coordinates);

            auto result = Y();

            // assemble elementary contributions
            for (auto e = 0; e < _manifold.nElements(); ++e) {
                for (auto q = 0; q < Q; ++q) {
                    result +=
                        values[{ e, q }] * _quadratureRule.getWeight(q) * _manifold.jacobian(e);
                }
            }

            return result;
        }

      private:
        // the quadrature rule
        static constexpr auto _quadratureRule = QuadratureRule::Get();
        // the number of quadrature points
        static constexpr int Q = _quadratureRule.size();
        // the domain of integration
        const manifold_t & _manifold;
        // the coordinates of the quadrature points in the domain of integration
        fem::quadrature_field_t<Q, vector_t<D>> _coordinates;
    };

}    // namespace  mito

#endif    // mito_quadrature_Integrator_h

// end of file
