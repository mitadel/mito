// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // Class {CellIntegrator} can integrate arbitrary tensors on a parametrized cell
    template <class quadratureRuleT, class parametrizedElementT>
    class CellIntegrator {
      public:
        // the quadrature rule type
        using quadrature_rule_type = quadratureRuleT;
        // the parametrized element type
        using parametrized_element_type = parametrizedElementT;
        // the cell type
        using cell_type = typename parametrized_element_type::cell_type;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // the constructor
        constexpr CellIntegrator(const parametrized_element_type & parametrized_element) :
            _parametrized_element(parametrized_element)
        {}

      public:
        template <class F>
        auto integrate(F && f) const -> F::output_type
        {
            //
            using result_type = F::output_type;

            // initialize the result
            result_type result{};

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // get cell parametrization under the manifold's coordinate system
            const auto phi = _parametrized_element.parametrization();
            // compute the derivative of the cell parametrization
            const auto J = functions::derivative(phi);
            // get the manifold's metric volume form
            const auto w = _parametrized_element.metric_volume_form();

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the parametric coordinates of the quadrature point
                constexpr auto xi_q = quadrature_rule.point(q);
                // get the quadrature weight and scale it by the reference simplex area
                constexpr auto w_q =
                    quadrature_rule.weight(q) * cell_type::reference_simplex_type::measure;
                // construct the metric volume element at {x} by contracting the metric volume
                // form with the tangent vectors at {x}
                const auto dV = w(phi(xi_q))(tensor::columns(J(xi_q)));
                // assemble the elementary contribution
                result += f(xi_q) * w_q * dV;
            });

            // all done
            return result;
        }

      private:
        // the parametrized element
        const parametrized_element_type & _parametrized_element;
    };

}    // namespace mito


// end of file
