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
        // constructor
        ValueBlock(const coefficient_field_type & coefficient) : _coefficient(coefficient) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires(element_of_type_c<elementT, element_type>)
        auto compute(const elementT & element) const -> elementary_shape
        {
            // the parametric coordinates type
            using parametric_coordinates_type = typename elementT::parametric_coordinates_type;

            // the elementary vector
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape elementary_vector{};

                    // the number of nodes per element
                    constexpr int n_nodes = element_type::n_nodes;

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

      private:
        // the coefficient field
        coefficient_field_type _coefficient;
    };

}    // namespace mito


// end of file
