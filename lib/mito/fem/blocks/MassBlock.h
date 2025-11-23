// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class elementT, class quadratureRuleT>
    class MassBlock : public AssemblyBlock<elementT, tensor::matrix_t<elementT::n_nodes>> {

      public:
        // my template parameters
        using element_type = elementT;
        using elementary_block_type = tensor::matrix_t<element_type::n_nodes>;
        using quadrature_rule_type = quadratureRuleT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // compute the elementary contribution of this block
        auto compute(const element_type & element) const -> elementary_block_type override
        {
            // the number of nodes per element
            constexpr int n_nodes = element_type::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary matrix
            elementary_block_type elementary_matrix;

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the parametric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the quadrature weight at this point scaled with the area of the canonical simplex
                constexpr auto w =
                    element_type::canonical_element_type::area * quadrature_rule.weight(q);

                // precompute the common factor
                auto factor = w * tensor::determinant(element.jacobian()(xi));

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the spatial gradient of the element's a-th shape function at {xi}
                    auto phi_a = element.template shape<a>()(xi);
                    // loop on the nodes of the element
                    tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                        // evaluate the spatial gradient of the element's b-th shape function at
                        // {xi}
                        auto phi_b = element.template shape<b>()(xi);
                        // populate the elementary contribution to the matrix
                        elementary_matrix[{ a, b }] += factor * phi_a * phi_b;
                    });
                });
            });

            // all done
            return elementary_matrix;
        }
    };

}    // namespace mito


// end of file
