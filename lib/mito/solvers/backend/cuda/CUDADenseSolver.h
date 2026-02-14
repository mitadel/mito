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

        // reset the linear system (i.e. the host copy of the matrix, right-hand side and solution)
        auto reset_system() -> void override;

        // solve the linear system
        auto solve() -> void override;

      private:
        // initialize the cuSOLVER utilities
        auto _initialize_cusolver() -> void override;

        // destroy the cuSOLVER utilities
        auto _finalize_cusolver() -> void override;

        // allocate the host memory for the matrix, right-hand side, and solution
        auto _allocate_host_memory(size_t size) -> void override;

        // allocate the device memory for the matrix and right-hand side
        auto _allocate_device_memory(size_t size) -> void override;

      private:
        // host copy of the matrix
        HostArray<real_type> _h_matrix;
        // device copy of the matrix
        DeviceArray<real_type> _d_matrix;
        // cuSOLVER handle
        cusolverDnHandle_t _cusolver_handle;
    };
}


// end of file
