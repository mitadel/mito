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

    // value grad matrix block factory
    template <class elementT, fields::vector_field_c coefficientFieldT>
    constexpr auto value_gradient_block(const coefficientFieldT & coefficient);

    // grad grad matrix block
    template <class elementT, class quadratureRuleT, fields::tensor_field_c coefficientFieldT>
    using grad_grad_block_t = GradientGradientBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // grad grad matrix block factory
    template <class elementT, fields::tensor_field_c coefficientFieldT>
    constexpr auto grad_grad_block(const coefficientFieldT & coefficient);

    // value value matrix block
    template <class elementT, class quadratureRuleT>
    using value_value_block_t = ValueValueBlock<elementT, quadratureRuleT>;

    // value value matrix block factory
    template <class elementT>
    constexpr auto value_value_block();

    // value vector block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    using value_block_t = ValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value vector block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto value_block(const coefficientFieldT & coefficient);

    // L2 norm block
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    using l2_norm_block_t = L2NormBlock<elementT, quadratureRuleT, functionT>;

    // L2 norm block factory
    template <class elementT, int doe, functions::function_c functionT>
    constexpr auto l2_norm_block(const functionT & f);
}


// end of file
