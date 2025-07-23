// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // TODO: unify vector and matrix block and allow distinction through a template parameter

    // TODO: implement sum and subtraction operators for the blocks (only for blocks that result in
    // the same elementary type)

    template <class elementT, class quadratureRuleT>
    class GradGradBlock :
        public AssemblyBlock<elementT, tensor::matrix_t<elementT::n_nodes>, quadratureRuleT> {

      public:
        // my parent class
        using parent_type =
            AssemblyBlock<elementT, tensor::matrix_t<elementT::n_nodes>, quadratureRuleT>;

        // my template parameters
        using element_type = typename parent_type::element_type;
        using elementary_block_type = typename parent_type::elementary_block_type;
        using quadrature_rule_type = typename parent_type::quadrature_rule_type;

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
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = parent_type::quadrature_rule.point(q);

                // precompute the common factor
                auto factor = parent_type::quadrature_rule.weight(q)
                            * tensor::determinant(element.jacobian()(xi));

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the spatial gradient of the element's a-th shape function at {xi}
                    auto dphi_a = element.template gradient<a>()(xi);
                    // loop on the nodes of the element
                    tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                        // evaluate the spatial gradient of the element's b-th shape function at
                        // {xi}
                        auto dphi_b = element.template gradient<b>()(xi);
                        // populate the elementary contribution to the matrix
                        elementary_matrix[{ a, b }] += factor * dphi_a * dphi_b;
                    });
                });
            });

            // all done
            return elementary_matrix;
        }
    };

}    // namespace mito


// end of file
