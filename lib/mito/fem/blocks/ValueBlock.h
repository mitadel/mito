// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // TOFIX: the source does not need to be necessarily a scalar field, it can be some other field
    // see if we can use {field_c} instead of {scalar_field_c}
    template <fields::scalar_field_c coefficientFieldT>
    class ValueBlock {

      public:
        // the type of the elementary shape matrix for an element of type {elementT}
        template <class elementT>
        using elementary_shape_t = tensor::vector_t<elementT::n_nodes>;

        // the type of the coefficient field
        using coefficient_field_type = coefficientFieldT;

      public:
        // constructor
        ValueBlock(const coefficient_field_type & coefficient) : _coefficient(coefficient) {}

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

            // the elementary vector
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape_type elementary_vector{};

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
                        // populate the elementary contribution to the vector
                        elementary_vector[{ a }] = coefficient * phi_a;
                    });

                    // all done
                    return elementary_vector;
                }));
        }

        // compute the elementary contribution of this block
        template <class elementT>
        auto compute(const elementT & element) const -> elementary_shape_t<elementT>
        {
            return compute<elementT::degree>(element);
        }

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
