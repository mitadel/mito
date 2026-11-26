// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <tensor::scalar_c scalar, class blockT>
    class BlockProduct {

      public:
        // the scalar type
        using scalar_type = scalar;

        // the elementary shape type
        template <class elementT>
        using elementary_shape_t = typename blockT::template elementary_shape_t<elementT>;  

      public:
        // constructor
        constexpr BlockProduct(scalar_type factor, blockT block) : _factor(factor), _block(block) {}

        // compute the elementary contribution of this block
        template <class elementT>
        auto compute(const elementT & element) const
            -> blockT::template elementary_shape_t<elementT>
        {
            // return the product of the blocks with the scalar
            return _factor * _block.compute(element);
        }

      private:
        // the scalar factor
        scalar_type _factor;
        // the block to multiply
        blockT _block;
    };

}    // namespace mito


// end of file
