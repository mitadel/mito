// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // matrix block factory
    template <class quadratureRuleT>
    constexpr auto matrix_block()
    {
        // all done
        return matrix_block_t<quadratureRuleT>();
    }

    // rhs block factory
    template <class quadratureRuleT>
    constexpr auto vector_block()
    {
        // all done
        return vector_block_t<quadratureRuleT>();
    }

}


// end of file
