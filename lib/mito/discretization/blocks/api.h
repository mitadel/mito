// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // matrix block
    template <class quadratureRuleT, class manifoldT>
    using matrix_block_t = MatrixBlock<quadratureRuleT, manifoldT>;

    // matrix block factory
    template <class quadratureRuleT, class manifoldT>
    constexpr auto matrix_block(const manifoldT & manifold);

    // vector block
    template <class quadratureRuleT, class manifoldT>
    using vector_block_t = VectorBlock<quadratureRuleT, manifoldT>;

    // vector block factory
    template <class quadratureRuleT, class manifoldT>
    constexpr auto vector_block(const manifoldT & manifold);
}


// end of file
