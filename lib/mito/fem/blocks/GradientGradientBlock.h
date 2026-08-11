// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class finiteElementT, class quadratureRuleT, fields::tensor_field_c coefficientFieldT>
    class GradientGradientBlock {

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
        GradientGradientBlock(const coefficient_field_type & coefficient) :
            _coefficient(coefficient)
        {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_shape
        {
            // the parametric coordinates type
            using parametric_coordinates_type = typename elementT::parametric_coordinates_type;

            // the elementary matrix
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape elementary_matrix{};

                    // the number of nodes per element
                    constexpr int n_nodes = element_type::n_nodes;

                    // the coordinates of the quadrature point
                    auto x = element.parametrization()(xi);

                    // evaluate the coefficient at the quadrature point
                    auto coefficient = _coefficient(x);

                    // loop on the nodes of the element
                    tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                        // evaluate the spatial gradient of the a-th shape function at {xi}
                        auto dphi_a = element.template gradient<a>()(xi);
                        // loop on the nodes of the element
                        tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                            // evaluate the spatial gradient of the b-th shape function at {xi}
                            auto dphi_b = element.template gradient<b>()(xi);
                            // populate the elementary contribution to the matrix
                            elementary_matrix[{ a, b }] = dphi_a * (coefficient * dphi_b);
                        });
                    });

                    // all done
                    return elementary_matrix;
                }));
        }

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
