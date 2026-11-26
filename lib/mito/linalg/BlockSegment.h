// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg {

    // a run of consecutive dense blocks of equal size in a sparse matrix
    struct BlockSegment {
        // the number of rows/columns in a block
        std::size_t block_size;
        // the number of blocks in the block segment
        std::size_t n_blocks;
    };

}    // namespace mito


// end of file
