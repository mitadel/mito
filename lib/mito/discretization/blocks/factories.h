// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // matrix block factory
    template <class elementT, class quadratureRuleT>
    constexpr auto grad_grad_block()
    {
        // all done
        return grad_grad_block_t<elementT, quadratureRuleT>();
    }

    // source term block factory
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    constexpr auto source_term_block(const sourceFieldT & f)
    {
        // all done
        return source_term_block_t<elementT, quadratureRuleT, sourceFieldT>(f);
    }

    // L2 norm block factory
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    constexpr auto l2_norm_block(const functionT & f)
    {
        // all done
        return l2_norm_block_t<elementT, quadratureRuleT, functionT>(f);
    }

}


// end of file
