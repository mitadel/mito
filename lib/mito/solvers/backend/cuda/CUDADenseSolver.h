// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    class CUDADenseSolver {
      public:
        // constructor
        CUDADenseSolver();

        // destructor
        ~CUDADenseSolver();

      public:
        // initialize the CUDA dense solver
        auto initialize(size_t size) -> void;

        // finalize the CUDA dense solver
        auto finalize() -> void;

        // reset the linear system (i.e. the host copy of the matrix, right-hand side and solution)
        auto reset_system() -> void;

        // set (add or insert depending on the mode) the value of a matrix entry in the host copy
        auto set_matrix_value(size_t, size_t, const double, const InsertMode) -> void;

        // set (add or insert depending on the mode) the value of a right-hand side entry in the
        // host copy
        auto set_rhs_value(size_t, const double, const InsertMode) -> void;

        // finalize the linear system assembly
        auto finalize_assembly() -> void;

        // solve the linear system
        auto solve() -> void;

      private:
        // initialize the cuSOLVER utilities
        auto _initialize_cusolver() -> void;

        // destroy the cuSOLVER utilities
        auto _finalize_cusolver() -> void;

        // allocate the host memory for the matrix, right-hand side, and solution
        auto _allocate_host_memory(size_t) -> void;

        // allocate the device memory for the matrix and right-hand side
        auto _allocate_device_memory(size_t) -> void;

        // initialize the host data for the matrix, right-hand side, and solution
        auto _initialize_host_data(size_t) -> void;

        // deallocate the host memory for the matrix, right-hand side, and solution
        auto _free_host_memory() -> void;

        // deallocate the device memory for the matrix and right-hand side
        auto _free_device_memory() -> void;

        // check the validity of the index in the matrix and right-hand side
        auto _check_index_validity(size_t) const -> void;

      private:
        // host copy of the matrix
        double * _h_matrix;
        // host copy of the right-hand side
        double * _h_rhs;
        // host copy of the solution
        double * _h_solution;
        // device copy of the matrix
        double * _d_matrix;
        // device copy of the right-hand side
        double * _d_rhs;
        // size of the linear system
        size_t _size;
        // flag to indicate if the solver has been initialized
        bool _is_solver_initialized;
        // flag to indicate which type of host memory has been allocated
        // 0: no memory allocated
        // 1: pinned memory allocated
        // 2: pageable (regular) memory allocated
        int _allocated_host_memory_type;
        // flag to indicate if the system assembly has been finalized
        bool _is_assembly_finalized;
        // cuSOLVER handle
        cusolverDnHandle_t _cusolver_handle;
        // cuda stream
        cudaStream_t _cuda_stream;
    };
}


// get the template definitions
#define mito_solvers_backend_cuda_CUDADenseSolver_icc
#include "CUDADenseSolver.icc"
#undef mito_solvers_backend_cuda_CUDADenseSolver_icc


// end of file
