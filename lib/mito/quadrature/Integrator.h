// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::quadrature {

    // an integrator on a manifold {manifoldT} with a quadrature rule of type {quadratureT} and
    // degree of exactness {r}
    template <quadrature_formula quadratureT, int r, class manifoldT>
    class Integrator {

      public:
        // publish my template parameters
        using manifold_type = manifoldT;
        using cell_type = typename manifold_type::cell_type::simplex_type;
        using coordinates_type = typename manifold_type::coordinates_type;

      private:
        // quadrature_type, cell_type, and r identify a specific quadrature rule
        using quadrature_rule_type = quadrature_rule_t<quadratureT, cell_type, r>;
        // the quadrature rule
        static constexpr auto _quadratureRule = quadrature_rule_type();
        // the number of quadrature points
        static constexpr int Q = quadrature_rule_type::npoints;
        // the dimension of the physical space
        static constexpr int D = manifold_type::dim;

      private:
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
                        _manifold.parametrization(element, _quadratureRule.point(q));
                }
                ++e;
            }

            // all done
            return;
        }

      public:
        Integrator(const manifold_type & manifold) :
            _manifold(manifold),
            _coordinates(manifold.nElements(), "coordinates")
        {
            _computeQuadPointCoordinates();
        }

        auto integrate(const fields::scalar_field_c auto & f) const -> scalar_t
        {
            auto result = scalar_t(0.0);
            // assemble elementary contributions
            int e = 0;
            for (const auto & cell : _manifold.elements()) {
                for (auto q = 0; q < Q; ++q) {
                    auto point = _coordinates[{ e, q }];
                    result += f(point) * _quadratureRule.weight(q) * _manifold.volume(cell);
                }
                // increment element count
                ++e;
            }

            return result;
        }

      private:
        // the domain of integration
        const manifold_type & _manifold;
        // the coordinates of the quadrature points in the domain of integration
        fem::quadrature_field_t<Q, coordinates_type> _coordinates;
    };

}    // namespace  mito


// end of file
