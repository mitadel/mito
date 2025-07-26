// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // assembly block
    template <class elementT, class blockT>
    using assembly_block_t = AssemblyBlock<elementT, blockT>;

    // grad grad block
    template <class elementT, class quadratureRuleT>
    using grad_grad_block_t = GradGradBlock<elementT, quadratureRuleT>;

    // grad grad block factory
    template <class elementT, class quadratureRuleT>
    constexpr auto grad_grad_block();

    // source term block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    using source_term_block_t = SourceTermBlock<elementT, quadratureRuleT, sourceFieldT>;

    // source term block factory
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    constexpr auto source_term_block(const sourceFieldT & f);
}


// end of file
