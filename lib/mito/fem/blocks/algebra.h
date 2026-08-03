// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // the sum of block
    template <class... blockTs>
    constexpr auto sum(blockTs... blocks)
    {
        return BlockSum<blockTs...>(blocks...);
    }

    template <class leftBlockT, class rightBlockT>
    constexpr auto operator+(leftBlockT left_block, rightBlockT right_block)
    {
        return sum(left_block, right_block);
    }

    template <tensor::scalar_c scalarT, class blockT>
    constexpr auto operator*(scalarT scalar, blockT block)
    {
        return BlockProduct<scalarT, blockT>(scalar, block);
    }

}


// end of file
