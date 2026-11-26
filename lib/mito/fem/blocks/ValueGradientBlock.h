// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <fields::vector_field_c coefficientFieldT>
    class ValueGradientBlock {

      public:
        // the type of the elementary shape matrix for an element of type {elementT}
        template <class elementT>
        using elementary_shape_t = tensor::matrix_t<elementT::n_nodes>;

        // the type of the coefficient field
        using coefficient_field_type = coefficientFieldT;

      public:
        // constructor
        ValueGradientBlock(const coefficient_field_type & coefficient) : _coefficient(coefficient)
        {}

      public:
        // compute the elementary contribution of this block
        template <int doe, class elementT>
        auto compute(const elementT & element) const -> elementary_shape_t<elementT>
        {
            // the parametric coordinates type
            using parametric_coordinates_type = typename elementT::parametric_coordinates_type;

            // the quadrature rule type
            using quadrature_rule_type = gauss_rule_t<elementT, doe>;

            // the elementary shape matrix type
            using elementary_shape_type = elementary_shape_t<elementT>;

            // the elementary matrix
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape_type elementary_matrix{};

                    // the number of nodes per element
                    constexpr int n_nodes = elementT::n_nodes;

                    // the coordinates of the quadrature point
                    auto x = element.parametrization()(xi);

                    // evaluate the coefficient at the quadrature point
                    auto coefficient = _coefficient(x);

                    // loop on the nodes of the element
                    tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                        // evaluate the element's a-th shape function at {xi}
                        const auto phi_a = element.template shape<a>()(xi);
                        // loop on the nodes of the element
                        tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                            // evaluate the spatial gradient of the b-th shape function at {xi}
                            auto dphi_b = element.template gradient<b>()(xi);
                            // populate the elementary contribution to the matrix
                            elementary_matrix[{ a, b }] = phi_a * coefficient * dphi_b;
                        });
                    });

                    // all done
                    return elementary_matrix;
                }));
        }

        template <class elementT>
        auto compute(const elementT & element) const -> elementary_shape_t<elementT>
        {
            return compute<2 * elementT::degree - 1>(element);
        }

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
