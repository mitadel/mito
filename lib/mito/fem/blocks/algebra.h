// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // the sum of block
    template <class... blockTs>
    constexpr auto sum(blockTs...)
    {
        return BlockSum<blockTs...>{};
    }

    template <class leftBlockT, class rightBlockT>
    constexpr auto operator+(leftBlockT left_block, rightBlockT right_block)
    {
        return sum(left_block, right_block);
    }

    template <tensor::scalar_c scalarT, class blockT>
    constexpr auto operator*(scalarT scalar, blockT)
    {
        return BlockProduct<scalarT, blockT>(scalar);
    }

}


// end of file
