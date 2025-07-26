// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // TOFIX: the source does not need to be necessarily a scalar field, it can be some other field
    template <class elementT, class quadratureRuleT, fields::field_c fieldT>
    class L2NormBlock : public AssemblyBlock<elementT, tensor::scalar_t> {

      public:
        // my parent class
        using parent_type = AssemblyBlock<elementT, tensor::scalar_t>;

        // my template parameters
        using element_type = typename parent_type::element_type;
        using elementary_block_type = typename parent_type::elementary_block_type;
        using quadrature_rule_type = quadratureRuleT;

        // the type of the function to compute the L2 norm of
        using field_type = fieldT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // constructor
        L2NormBlock(const field_type & field) : _field(field) {}

      public:
        // compute the elementary contribution of this block
        auto compute(const element_type & element) const -> elementary_block_type override
        {
            // the number of nodes per element
            constexpr int n_nodes = element_type::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary contribution to the L2 norm
            elementary_block_type elementary_contribution;

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the coordinates of the quadrature point
                auto coord = element.parametrization()(xi);

                // populate the elementary contribution to the matrix
                elementary_contribution += quadrature_rule.weight(q)
                                         * tensor::determinant(element.jacobian()(xi))
                                         * _field(coord);
            });

            // all done
            return elementary_contribution;
        }

      private:
        // the field to compute the L2 norm of
        const field_type & _field;
    };

}    // namespace mito


// end of file
