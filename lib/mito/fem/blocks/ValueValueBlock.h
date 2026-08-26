// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class finiteElementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    class ValueValueBlock {

      public:
        // my finite element type
        using element_type = finiteElementT;
        // my quadrature rule
        using quadrature_rule_type = quadratureRuleT;
        // my elementary shape
        using elementary_shape = tensor::matrix_t<element_type::n_nodes>;

        // the type of the coefficient field
        using coefficient_field_type = coefficientFieldT;

      public:
        // constructor
        ValueValueBlock(const coefficient_field_type & coefficient) : _coefficient(coefficient) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires(element_of_type_c<elementT, element_type>)
        auto compute(const elementT & element) const -> elementary_shape
        {
            // CACHING EXPERIMENT: evaluate all geometric quantities exactly once per quadrature
            // point as plain tensors

            // the number of nodes per element
            constexpr int n_nodes = element_type::n_nodes;
            // the number of quadrature points
            constexpr int n_quads = quadrature_rule_type::npoints;
            // the quadrature rule
            constexpr auto quadrature_rule = quadrature_rule_type();
            // the measure of the reference simplex
            constexpr auto measure = element_type::mesh_cell_type::reference_simplex_type::measure;

            // the geometry of the element
            const auto & parametrized_element = element.element();
            const auto & phi = parametrized_element.parametrization();
            const auto J_fn = parametrized_element.jacobian();
            const auto & w = parametrized_element.metric_volume_form();

            // the elementary matrix
            elementary_shape elementary_matrix{};

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                constexpr auto xi = quadrature_rule.point(q);
                constexpr auto w_q = quadrature_rule.weight(q) * measure;

                // evaluate the geometry once at this quadrature point
                const auto x = phi(xi);
                const auto J = J_fn(xi);
                const auto dV = w(x)(tensor::columns(J));
                const auto coefficient = _coefficient(x);

                // the shape function values at this quadrature point
                const auto phi_values = [&]<int... a>(tensor::integer_sequence<a...>) {
                    return std::array{ elementT::shape_functions.template shape<a>()(xi)... };
                }(tensor::make_integer_sequence<n_nodes>{});

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                        elementary_matrix[{ a, b }] +=
                            w_q * dV * coefficient * phi_values[a] * phi_values[b];
                    });
                });
            });

            // all done
            return elementary_matrix;
        }

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
