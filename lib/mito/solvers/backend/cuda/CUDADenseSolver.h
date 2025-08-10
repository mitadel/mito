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

    template <real_c realT>
    class CUDADenseSolver : public CUDASolver<realT> {
      private:
        // real type definition from CUDASolver
        using real_type = typename CUDASolver<realT>::real_type;

      public:
        // constructor
        CUDADenseSolver(SolverType solver_type = SolverType::LU);

        // destructor
        ~CUDADenseSolver();

      public:
        // set (add or insert depending on the mode) the value of a matrix entry in the host copy
        auto set_matrix_value(
            size_t row, size_t col, const real_type value,
            const InsertMode insert_mode = InsertMode::INSERT_VALUE) -> void override;

        // set (add or insert depending on the mode) the value of a right-hand side entry in the
        // host copy
        auto set_rhs_value(
            size_t row, const real_type value,
            const InsertMode insert_mode = InsertMode::INSERT_VALUE) -> void override;

        // solve the linear system
        auto solve() -> void override;

        // get the solution vector
        auto get_solution(std::vector<real_type> & solution) const -> void override;

      private:
        // initialize the cuSOLVER utilities
        auto _initialize_cusolver() -> void override;

        // destroy the cuSOLVER utilities
        auto _finalize_cusolver() -> void override;

        // allocate the host memory for the matrix, right-hand side, and solution
        auto _allocate_host_memory(size_t size) -> void override;

        // allocate the device memory for the matrix and right-hand side
        auto _allocate_device_memory(size_t size) -> void override;

        // initialize the host data for the matrix, right-hand side, and solution
        auto _initialize_host_data(size_t size) -> void override;

        // deallocate the host memory for the matrix, right-hand side, and solution
        auto _free_host_memory() -> void override;

        // deallocate the device memory for the matrix and right-hand side
        auto _free_device_memory() -> void override;

      private:
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
        // flag to indicate which type of host memory has been allocated
        // 0: no memory allocated
        // 1: pinned memory allocated
        // 2: pageable (regular) memory allocated
        int _allocated_host_memory_type;
        // cuSOLVER handle
        cusolverDnHandle_t _cusolver_handle;
    };
}


// get the template definitions
#define mito_solvers_backend_cuda_CUDADenseSolver_icc
#include "CUDADenseSolver.icc"
#undef mito_solvers_backend_cuda_CUDADenseSolver_icc


// end of file
