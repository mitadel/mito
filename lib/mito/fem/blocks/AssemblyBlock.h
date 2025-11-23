// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // TODO: implement sum and subtraction operators for the blocks (only for blocks that result in
    // the same elementary type)

    template <class elementT, class blockT>
    class AssemblyBlock {

      public:
        // my template parameters
        using element_type = elementT;
        using elementary_block_type = blockT;

      public:
        // the constructor
        constexpr AssemblyBlock() = default;

        // destructor
        constexpr ~AssemblyBlock() = default;

        // delete move constructor
        constexpr AssemblyBlock(AssemblyBlock &&) noexcept = delete;

        // delete copy constructor
        constexpr AssemblyBlock(const AssemblyBlock &) = delete;

        // delete assignment operator
        constexpr AssemblyBlock & operator=(const AssemblyBlock &) = delete;

        // delete move assignment operator
        constexpr AssemblyBlock & operator=(AssemblyBlock &&) noexcept = delete;

      public:
        // compute the elementary contribution of this block
        virtual auto compute(const element_type & element) const -> elementary_block_type = 0;
    };

}    // namespace mito


// end of file
