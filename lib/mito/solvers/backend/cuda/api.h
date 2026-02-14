// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // cuda dense solver
    template <real_c realT>
    using dense_t = CUDADenseSolver<realT>;

    // cuda sparse solver
    template <real_c realT>
    using sparse_t = CUDASparseSolver<realT>;
}


// end of file