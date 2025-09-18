// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {
    template <real_c realT>
    class CUDASparseSolver : public CUDASolver<realT> {
      private:
        // real type definition from CUDASolver
        using real_type = typename CUDASolver<realT>::real_type;

      public:
        // constructor
        CUDASparseSolver(SolverType solver_type = SolverType::LU);

        // destructor
        ~CUDASparseSolver();

      public:
        // initialize the CUDA sparse solver
        auto initialize(size_t size, size_t nnz = -1) -> void;

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
        // host copy of the matrix in CSR format
        Eigen::SparseMatrix<real_type, Eigen::RowMajor> _h_matrix;
        // device copy of the matrix in CSR format
        DeviceSparseMatrix<real_type> _d_matrix;
        // device copy of the solution
        DeviceArray<real_type> _d_solution;
        // cuDSS handle
        cudssHandle_t _cudss_handle;
    };
}


// end of file
