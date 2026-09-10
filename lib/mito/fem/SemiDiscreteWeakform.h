// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class blockT>
    struct StiffnessMixin {
        // the elementary shape type
        using elementary_shape = typename blockT::elementary_shape;
        // the element type
        using element_type = typename blockT::element_type;

        // compute the elementary contributions to stiffness matrix
        template <class elementT>
        auto compute_stiffness(const elementT & e) const -> typename blockT::elementary_shape
        {
            return block.compute(e);
        }

        // the stiffness assembly block
        blockT block;
    };

    template <class blockT>
    struct LoadMixin {
        // the elementary shape type
        using elementary_shape = typename blockT::elementary_shape;
        // the element type
        using element_type = typename blockT::element_type;

        // compute the elementary contributions to load vector
        template <class elementT>
        auto compute_load(const elementT & e) const -> typename blockT::elementary_shape
        {
            return block.compute(e);
        }

        // the load assembly block
        blockT block;
    };


    template <class... mixinTs>
    requires same_finite_element_blocks_c<mixinTs...>
    class SemiDiscreteWeakform : public mixinTs... {

      public:
        // constructor
        constexpr SemiDiscreteWeakform(const mixinTs &... mixins) : mixinTs(mixins)... {}

        // destructor
        constexpr ~SemiDiscreteWeakform() = default;

        // delete move constructor
        constexpr SemiDiscreteWeakform(SemiDiscreteWeakform &&) noexcept = delete;

        // delete copy constructor
        constexpr SemiDiscreteWeakform(const SemiDiscreteWeakform &) = delete;

        // delete assignment operator
        constexpr SemiDiscreteWeakform & operator=(const SemiDiscreteWeakform &) = delete;

        // delete move assignment operator
        constexpr SemiDiscreteWeakform & operator=(SemiDiscreteWeakform &&) noexcept = delete;
    };

}    // namespace mito


// end of file
