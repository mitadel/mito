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
        using reference_cell_type = typename manifold_type::cell_type::reference_simplex_type;
        using coordinates_type = typename manifold_type::coordinates_type;

      private:
        // quadrature_type, cell_type, and r identify a specific quadrature rule
        using quadrature_rule_type = quadrature_rule_t<quadratureT, reference_cell_type, r>;
        // the quadrature rule
        static constexpr auto _quadratureRule = quadrature_rule_type();
        // the number of quadrature points
        static constexpr int Q = quadrature_rule_type::npoints;
        // the quadrature field type to store the coordinates of the quadrature points
        using quadrature_field_type = discrete::quadrature_field_t<cell_type, Q, coordinates_type>;

      private:
        template <int... q>
        auto _computeQuadPointCoordinates(tensor::integer_sequence<q...>) -> void
        {
            // loop on elements
            for (const auto & element : _manifold.elements()) {
                // use element parametrization and manifold's coordinate systemto map the position
                // of quadrature points in the canonical element to the coordinate of the quadrature
                // point
                _coordinates.insert(
                    element.simplex(),
                    { element.parametrization(
                        _quadratureRule.point(q), _manifold.coordinate_system())... });
            }

            // all done
            return;
        }

      public:
        Integrator(const manifold_type & manifold) :
            _manifold(manifold),
            _coordinates("coordinates")
        {
            _computeQuadPointCoordinates(tensor::make_integer_sequence<Q>{});
        }

        auto integrate(const fields::scalar_field_c auto & f) const -> tensor::scalar_t
        {
            auto result = tensor::scalar_t{ 0.0 };
            // assemble elementary contributions
            for (const auto & cell : _manifold.elements()) {
                for (auto q = 0; q < Q; ++q) {
                    auto point = _coordinates(cell.simplex())[q];
                    result += f(point) * _quadratureRule.weight(q) * _manifold.volume(cell);
                }
            }

            return result;
        }

      private:
        // the domain of integration
        const manifold_type & _manifold;
        // the coordinates of the quadrature points in the domain of integration
        quadrature_field_type _coordinates;
    };

}    // namespace  mito


// end of file
