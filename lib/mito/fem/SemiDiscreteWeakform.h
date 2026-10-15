// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class... termTs>
    class SemiDiscreteWeakform : public termTs... {

      public:
        // extend the getters to this class
        using termTs::get...;

        // extend the compute_blocks to this class
        using termTs::compute_block...;

        // constructor
        constexpr explicit SemiDiscreteWeakform(termTs... term) : termTs(std::move(term))... {}
    };

}    // namespace mito


// end of file
