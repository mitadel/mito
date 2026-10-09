// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // value grad matrix block
    template <fields::vector_field_c coefficientFieldT>
    using value_gradient_block_t = ValueGradientBlock<coefficientFieldT>;

    // value gradient matrix block factory
    template <fields::vector_field_c coefficientFieldT>
    constexpr auto value_gradient_block(const coefficientFieldT & coefficient)
    {
        // all done
        return value_gradient_block_t<coefficientFieldT>(coefficient);
    }

    // grad grad matrix block
    template <fields::tensor_field_c coefficientFieldT>
    using grad_grad_block_t = GradientGradientBlock<coefficientFieldT>;

    // grad grad matrix block factory
    template <fields::tensor_field_c coefficientFieldT>
    constexpr auto grad_grad_block(const coefficientFieldT & coefficient)
    {
        // all done
        return grad_grad_block_t<coefficientFieldT>(coefficient);
    }

    // value value matrix block
    template <fields::scalar_field_c coefficientFieldT>
    using value_value_block_t = ValueValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value value matrix block factory
    template <fields::scalar_field_c coefficientFieldT>
    constexpr auto value_value_block(const coefficientFieldT & coefficient)
    {
        // all done
        return value_value_block_t<coefficientFieldT>(coefficient);
    }

    // value vector block
    template <fields::scalar_field_c coefficientFieldT>
    using value_block_t = ValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value vector block factory
    template <fields::scalar_field_c coefficientFieldT>
    constexpr auto value_block(const coefficientFieldT & coefficient)
    {
        // all done
        return value_block_t<coefficientFieldT>(coefficient);
    }

    // L2 norm block
    template <functions::function_c functionT>
    using l2_norm_block_t = L2NormBlock<elementT, quadratureRuleT, functionT>;

    // L2 norm block factory
    template <functions::function_c functionT>
    constexpr auto l2_norm(const functionT & f)
    {
        // all done
        return l2_norm_block_t<functionT>(f);
    }

}


// end of file
