// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // TOFIX: the source does not need to be necessarily a scalar field, it can be some other field
    // see if we can use {field_c} instead of {scalar_field_c}
    template <class finiteElementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    class ValueBlock {

      public:
        // my finite element type
        using element_type = finiteElementT;
        // my quadrature rule
        using quadrature_rule_type = quadratureRuleT;
        // my elementary shape
        using elementary_shape = tensor::vector_t<element_type::n_nodes>;

        // the type of the coefficient field
        using coefficient_field_type = coefficientFieldT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // constructor
        ValueBlock(const coefficient_field_type & coefficient) : _coefficient(coefficient) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires(element_of_type_c<elementT, element_type>)
        auto compute(const elementT & element) const -> elementary_shape
        {
            // the number of nodes per element
            constexpr int n_nodes = element_type::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary vector
            elementary_shape elementary_vector{};

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the parametric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the coordinates of the quadrature point
                auto coord = element.parametrization()(xi);

                // the measure of the canonical simplex
                constexpr auto measure =
                    element_type::mesh_cell_type::reference_simplex_type::measure;

                // the quadrature weight at this point scaled with the area of the canonical simplex
                constexpr auto w = measure * quadrature_rule.weight(q);

                // precompute the common factor
                auto factor = w * _coefficient(coord) * element.volume_element()(xi);

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the a-th shape function at {xi}
                    auto phi_a = element.template shape<a>()(xi);
                    // populate the elementary contribution to the vector
                    elementary_vector[{ a }] += factor * phi_a;
                });
            });

            // all done
            return elementary_vector;
        }

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
