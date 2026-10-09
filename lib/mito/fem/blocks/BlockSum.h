// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class firstBlockT, class... blockTs>
    class BlockSum {

      public:
        // the elementary shape type
        template <class elementT>
        using elementary_shape_t = typename firstBlockT::template elementary_shape_t<elementT>;

        // the constructor
        constexpr BlockSum(firstBlockT first_block, blockTs... blocks) :
            _blocks(std::move(first_block), std::move(blocks)...)
        {}

        // compute the elementary contribution of this block
        template <class elementT>
        requires same_elementary_shape_c<elementT, firstBlockT, blockTs...>
        auto compute(const elementT & element) const
            -> firstBlockT::template elementary_shape_t<elementT>
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
