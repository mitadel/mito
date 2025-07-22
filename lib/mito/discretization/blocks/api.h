// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // matrix block
    template <class quadratureRuleT>
    using matrix_block_t = MatrixBlock<quadratureRuleT>;

    // matrix block factory
    template <class quadratureRuleT>
    constexpr auto matrix_block();

    // vector block
    template <class quadratureRuleT>
    using vector_block_t = VectorBlock<quadratureRuleT>;

    // vector block factory
    template <class quadratureRuleT>
    constexpr auto vector_block();
}


// end of file
