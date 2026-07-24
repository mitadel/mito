// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class bulkElementT, class interfaceElementT, class blockT>
    class InterfaceCouplingBlock {

      public:
        // template parameters
        using bulk_element_type = bulkElementT;
        using interface_element_type = interfaceElementT;
        using elementary_block_type = blockT;

      public:
        // the constructor
        constexpr InterfaceCouplingBlock() = default;

        // destructor
        constexpr ~InterfaceCouplingBlock() = default;

        // delete move constructor
        constexpr InterfaceCouplingBlock(InterfaceCouplingBlock &&) noexcept = delete;

        // delete copy constructor
        constexpr InterfaceCouplingBlock(const InterfaceCouplingBlock &) = delete;

        // delete assignment operator
        constexpr InterfaceCouplingBlock & operator=(const InterfaceCouplingBlock &) = delete;

        // delete move assignment operator
        constexpr InterfaceCouplingBlock & operator=(InterfaceCouplingBlock &&) noexcept = delete;

      public:
        // elementary contribution coupling interface_element, bulk_positive, and bulk_negative
        virtual auto compute(
            const interface_element_type & interface_element,
            const bulk_element_type & bulk_positive, const bulk_element_type & bulk_negative) const
            -> elementary_block_type = 0;
    };

}    // namespace mito::fem::blocks


// end of file
