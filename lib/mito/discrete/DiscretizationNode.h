// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// code guard
#pragma once

/*
 * This class represents a discretization node in D-dimensional space.
 *
 */

namespace mito::discrete {

    class DiscretizationNode : public utilities::Invalidatable {

      public:
        // default constructor
        constexpr DiscretizationNode() = default;

        // move constructor
        constexpr DiscretizationNode(DiscretizationNode &&) noexcept = default;

        // copy constructor
        constexpr DiscretizationNode(const DiscretizationNode &) = default;

        // destructor
        constexpr ~DiscretizationNode() = default;

        // default assignment operator
        constexpr DiscretizationNode & operator=(const DiscretizationNode &) = default;

        // default move assignment operator
        constexpr DiscretizationNode & operator=(DiscretizationNode &&) noexcept = default;
    };

}


// end of file
