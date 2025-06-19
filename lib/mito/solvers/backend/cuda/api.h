// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // cuda dense solver
    template<typename realT>
    using dense_t = CUDADenseSolver<realT>;

}


// end of file