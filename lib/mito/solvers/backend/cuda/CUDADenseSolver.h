// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// a struct to hold the cusolver function pointers for different data types
template <typename T>
struct cusolver_traits;

template <>
struct cusolver_traits<double> {
    // Cholesky factorization and solve routines for double precision
    static constexpr auto potrf_buffer_size = cusolverDnDpotrf_bufferSize;
    static constexpr auto potrf = cusolverDnDpotrf;
    static constexpr auto potrs = cusolverDnDpotrs;
    // LU factorization and solve routines for double precision
    static constexpr auto getrf_buffer_size = cusolverDnDgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnDgetrf;
    static constexpr auto getrs = cusolverDnDgetrs;
};

template <>
struct cusolver_traits<float> {
    // Cholesky factorization and solve routines for single precision
    static constexpr auto potrf_buffer_size = cusolverDnSpotrf_bufferSize;
    static constexpr auto potrf = cusolverDnSpotrf;
    static constexpr auto potrs = cusolverDnSpotrs;
    // LU factorization and solve routines for single precision
    static constexpr auto getrf_buffer_size = cusolverDnSgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnSgetrf;
    static constexpr auto getrs = cusolverDnSgetrs;
};


namespace mito::solvers::cuda {

    template<real_c realT>
    class CUDADenseSolver {
        public:
        // type alias for real numbers
        using real_type = realT;

      public:
        // constructor
        CUDADenseSolver(SolverType);

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
        auto set_matrix_value(size_t, size_t, const real_type, const InsertMode) -> void;

        // set (add or insert depending on the mode) the value of a right-hand side entry in the
        // host copy
        auto set_rhs_value(size_t, const real_type, const InsertMode) -> void;

        // finalize the linear system assembly
        auto finalize_assembly() -> void;

        // solve the linear system
        auto solve() -> void;

        // get the solution vector
        template <class solutionT>
        auto get_solution(solutionT & solution) const -> void;

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
        // solver type
        SolverType _solver_type;
        // host copy of the matrix
        real_type * _h_matrix;
        // host copy of the right-hand side
        real_type * _h_rhs;
        // host copy of the solution
        real_type * _h_solution;
        // device copy of the matrix
        real_type * _d_matrix;
        // device copy of the right-hand side
        real_type * _d_rhs;
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
