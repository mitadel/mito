// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // matrix block factory
    template <class quadratureRuleT, class manifoldT>
    constexpr auto matrix_block(const manifoldT & manifold)
    {
        // all done
        return matrix_block_t<quadratureRuleT, manifoldT>(manifold);
    }

    // rhs block factory
    template <class quadratureRuleT, class manifoldT>
    constexpr auto vector_block(const manifoldT & manifold)
    {
        // all done
        return vector_block_t<quadratureRuleT, manifoldT>(manifold);
    }

}


// end of file
