// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg {

    // block segment
    using block_segment_t = BlockSegment;

    // enumeration for the assembly format
    enum class assembly_format { hash, csr, coo };

    // enumeration indicating how the sparsity pattern of a matrix compares to another matrix
    enum class sparsity_pattern { same, subset, different, unknown };

    // enumeration indicating the duplication policy for a matrix or vector
    enum class duplication_policy { copy_pattern };
}


// end of file
