// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
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
        using cell_type = typename manifold_type::cell_type;
        using reference_cell_type = typename manifold_type::cell_type::reference_simplex_type;
        using coordinates_type = typename manifold_type::coordinates_type;

      private:
        // quadrature_type, cell_type, and r identify a specific quadrature rule
        using quadrature_rule_type = quadrature_rule_t<quadratureT, reference_cell_type, r>;
        // the quadrature rule
        static constexpr auto _quadratureRule = quadrature_rule_type();
        // the number of quadrature points
        static constexpr int Q = quadrature_rule_type::npoints;

      public:
        // the constructor
        Integrator(const manifold_type & manifold) : _manifold(manifold) {}

        // integrate field {f}
        auto integrate(const fields::scalar_field_c auto & f) const -> tensor::scalar_t
        {
            // initialize the result
            auto result = tensor::scalar_t{ 0.0 };
            // assemble elementary contributions
            for (const auto & cell : _manifold.elements()) {
                // get cell parametrization under the manifold's coordinate system
                const auto phi = cell.parametrization();
                // compute the derivative of the cell parametrization
                const auto J = functions::derivative(phi);
                // get the manifold's metric volume form
                const auto w = cell.metric_volume_form();
                // loop on quadrature points
                for (auto q = 0; q < Q; ++q) {
                    // get the quadrature point coordinates in physical space
                    const auto x_q = _quadratureRule.point(q);
                    // get the quadrature weight and scale it by the reference simplex area
                    const auto w_q =
                        _quadratureRule.weight(q) * cell_type::reference_simplex_type::measure;
                    // construct the metric volume element at {x} by contracting the metric volume
                    // form with the tangent vectors at {x}
                    const auto dV = w(phi(x_q))(tensor::columns(J(x_q)));
                    // assemble the elementary contribution
                    result += f(phi(x_q)) * w_q * dV;
                }
            }

            // all done
            return result;
        }

      private:
        // the domain of integration
        const manifold_type & _manifold;
    };

}    // namespace  mito


// end of file
