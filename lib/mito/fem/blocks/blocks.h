// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // value grad matrix block
    template <class elementT, class quadratureRuleT, fields::vector_field_c coefficientFieldT>
    using value_gradient_block_t = ValueGradientBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value gradient matrix block factory
    template <class elementT, int doe, fields::vector_field_c coefficientFieldT>
    constexpr auto value_gradient_block(const coefficientFieldT & coefficient)
    {
        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_gradient_block_t<elementT, quadrature_rule_type, coefficientFieldT>(
            coefficient);
    }

    // grad grad matrix block
    template <class elementT, class quadratureRuleT, fields::tensor_field_c coefficientFieldT>
    using grad_grad_block_t = GradientGradientBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // grad grad matrix block factory
    template <class elementT, int doe, fields::tensor_field_c coefficientFieldT>
    constexpr auto grad_grad_block(const coefficientFieldT & coefficient)
    {
        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return grad_grad_block_t<elementT, quadrature_rule_type, coefficientFieldT>(coefficient);
    }

    // value value matrix block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    using value_value_block_t = ValueValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value value matrix block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto value_value_block(const coefficientFieldT & coefficient)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_value_block_t<elementT, quadrature_rule_type, coefficientFieldT>(coefficient);
    }

    // value vector block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    using value_block_t = ValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value vector block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto value_block(const coefficientFieldT & coefficient)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_block_t<elementT, quadrature_rule_type, coefficientFieldT>(coefficient);
    }

    // L2 norm block
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    using l2_norm_block_t = L2NormBlock<elementT, quadratureRuleT, functionT>;

    // L2 norm block factory
    template <class elementT, int doe = 2 * elementT::degree, functions::function_c functionT>
    constexpr auto l2_norm(const functionT & f)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return l2_norm_block_t<elementT, quadrature_rule_type, functionT>(f);
    }

}


// end of file
