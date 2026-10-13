// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // an ordinary block computed on the trace of an interface element, so that
    // {InterfaceTraceElement} stays an implementation detail of the interface blocks
    template <class blockT, trace_operator_t opT>
    class InterfaceBlock {

      public:
        // the block computed on the trace
        using block_type = blockT;
        // my finite element type
        using element_type = typename block_type::element_type;
        // my elementary shape
        using elementary_shape = typename block_type::elementary_shape;

      public:
        // constructor
        constexpr InterfaceBlock(const block_type & block) : _block(block) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires(element_of_type_c<elementT, element_type>)
        auto compute(const elementT & element) const -> elementary_shape
        {
            // view the element through the trace operator and delegate
            return _block.compute(InterfaceTraceElement<elementT, opT>(element));
        }

      private:
        // the block computed on the trace
        block_type _block;
    };

}    // namespace mito::fem::blocks


// end of file
