// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    // require that {functionT} is a function in barycentric coordinates
    requires(std::is_same_v<
             typename functionT::input_type, typename quadratureRuleT::quadrature_point_type>)
    class L2NormBlock : public AssemblyBlock<elementT, tensor::scalar_t> {

      public:
        // my parent class
        using parent_type = AssemblyBlock<elementT, tensor::scalar_t>;

        // my template parameters
        using element_type = typename parent_type::element_type;
        using elementary_block_type = typename parent_type::elementary_block_type;
        using quadrature_rule_type = quadratureRuleT;

        // the type of the function to compute the L2 norm of
        using function_type = functionT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // constructor
        L2NormBlock(const function_type & function) : _function(function) {}

      public:
        // compute the elementary contribution of this block
        auto compute(const element_type & element) const -> elementary_block_type override
        {
            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary contribution to the L2 norm
            auto elementary_contribution = elementary_block_type{};

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // populate the elementary contribution to the matrix
                elementary_contribution += quadrature_rule.weight(q)
                                         * tensor::determinant(element.jacobian()(xi))
                                         * _function(xi) * _function(xi);
            });

            // all done
            return elementary_contribution;
        }

      private:
        // the function to compute the L2 norm of
        const function_type & _function;
    };

}    // namespace mito


// end of file
