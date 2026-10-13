// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class... Terms>
    class SemiDiscreteWeakform : public Terms... {

      public:
        // extend the getters to this class
        using Terms::get...;

        // extend the compute_blocks to this class
        using Terms::compute_block...;
    };

}    // namespace mito


// end of file
