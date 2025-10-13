// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // assembly block
    template <class elementT, class blockT>
    using assembly_block_t = AssemblyBlock<elementT, blockT>;

    // grad grad block
    template <class elementT, class quadratureRuleT>
    using grad_grad_block_t = GradGradBlock<elementT, quadratureRuleT>;

    // grad grad block factory
    template <class elementT, class quadratureRuleT>
    constexpr auto grad_grad_block();

    // mass block
    template <class elementT, class quadratureRuleT>
    using mass_block_t = MassBlock<elementT, quadratureRuleT>;

    // mass block factory
    template <class elementT, class quadratureRuleT>
    constexpr auto mass_block();

    // source term block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    using source_term_block_t = SourceTermBlock<elementT, quadratureRuleT, sourceFieldT>;

    // source term block factory
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    constexpr auto source_term_block(const sourceFieldT & f);

    // L2 norm block
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    using l2_norm_block_t = L2NormBlock<elementT, quadratureRuleT, functionT>;

    // L2 norm block factory
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    constexpr auto l2_norm_block(const functionT & f);
}


// end of file
