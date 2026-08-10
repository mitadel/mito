// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class firstBlockT, class... blockTs>
    requires(
        same_finite_element_blocks_c<firstBlockT, blockTs...>
        && same_elementary_shape_blocks_c<firstBlockT, blockTs...>)
    class BlockSum {

      public:
        // my finite element type
        using element_type = typename firstBlockT::element_type;
        // my elementary shape
        using elementary_shape = typename firstBlockT::elementary_shape;

      public:
        // the constructor
        constexpr BlockSum(firstBlockT first_block, blockTs... blocks) :
            _blocks(std::move(first_block), std::move(blocks)...)
        {}

        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_shape
        {
            // return the sum of all the blocks
            return std::apply(
                [&](const auto &... blocks) { return (blocks.compute(element) + ...); }, _blocks);
        }

      private:
        // the blocks to sum
        std::tuple<firstBlockT, blockTs...> _blocks;
    };

}    // namespace mito


// end of file
