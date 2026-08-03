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
        // my finite element type
        using element_type = typename blockT::element_type;
        // my elementary shape
        using elementary_shape = typename blockT::elementary_shape;
        // the scalar type
        using scalar_type = scalar;

      public:
        // constructor
        constexpr BlockProduct(scalar_type factor) : _factor(factor) {}

        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_shape
        {
            // return the product of the blocks with the scalar
            return _factor * blockT{}.compute(element);
        }

      private:
        // the scalar factor
        scalar_type _factor;
    };

}    // namespace mito


// end of file
