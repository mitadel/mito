// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <typename tagT, typename blockT>
    class Term {

      public:
        // the tag associated to this term
        using tag_type = tagT;

        // the type of the block associated to this term
        using block_type = blockT;

        // constructor
        constexpr explicit Term(block_type block) : _block(std::move(block)) {}

        // the type of the elementary shape associated to this term
        template <typename elementT>
        using elementary_shape_type = typename block_type::template elementary_shape_t<elementT>;

        // get the block associated to this term
        template <std::same_as<tagT> T>
        block_type & get()
        {
            return _block;
        }

        // get the block associated to this term
        template <std::same_as<tagT> T>
        const block_type & get() const
        {
            return _block;
        }

        // compute the elementary shape associated to this term
        template <std::same_as<tagT> T, typename elementT>
        constexpr auto compute_block(const elementT & element) const
            -> elementary_shape_type<elementT>
        {
            return _block.compute(element);
        }

      private:
        // the block associated to this term
        block_type _block;
    };

}    // namespace mito


// end of file
