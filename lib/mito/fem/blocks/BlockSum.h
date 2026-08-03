// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // TODO: add a benchmark that there is no overhead in defining the block wrt summing the
    // resulting matrix entries

    template <class firstBlockT, class... blockTs>
    requires(
        same_finite_element_blocks_c<firstBlockT, blockTs...>
        && same_elementary_shape_blocks_c<firstBlockT, blockTs...>)
    class BlockSum {

      public:
        // my template parameters
        using element_type = typename firstBlockT::element_type;
        using elementary_block_type = typename firstBlockT::elementary_block_type;

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_block_type
        {
            // return the sum of all the blocks
            return (firstBlockT{}.compute(element) + ... + blockTs{}.compute(element));
        }
    };

}    // namespace mito


// end of file
