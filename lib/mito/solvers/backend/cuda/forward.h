// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // available insert modes for matrix and right-hand side entries
    enum InsertMode { ADD_VALUE, INSERT_VALUE };

    // available solver types
    enum SolverType { CHOLESKY, LU };

    // class for CUDA dense solver
    template<real_c realT>
    class CUDADenseSolver;
}


// end of file
