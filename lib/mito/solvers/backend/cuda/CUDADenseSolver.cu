// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDADenseSolver<realT>::CUDADenseSolver(SolverType solver_type) :
    CUDASolver<realT>(solver_type),
    _h_matrix(),
    _d_matrix(),
    _cusolver_handle()
{
    // initialize cuSOLVER
    _initialize_cusolver();
}

// destructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDADenseSolver<realT>::~CUDADenseSolver()
{
    // finalize cuSOLVER
    _finalize_cusolver();
}

// add/insert {value} to matrix entry at ({row}, {col}) of the host copy
template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::set_matrix_value(
    size_t row, size_t col, const real_type value, const InsertMode insert_mode) -> void
{
    // check if the system assembly is finalized and throw an error if it is
    if (this->_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is already finalized. Cannot add/insert values to the matrix.");
    }

    // check if the row and column indices are within bounds
    this->_check_index_validity(row);
    this->_check_index_validity(col);

    // add/insert the value to the matrix entry in the host matrix
    // NOTE: We store the matrix in column-major order since the cuSOLVER library expects the matrix
    // to be in column-major order.
    if (insert_mode == InsertMode::ADD_VALUE)
        _h_matrix[col * this->_size + row] += value;
    else if (insert_mode == InsertMode::INSERT_VALUE)
        _h_matrix[col * this->_size + row] = value;
    else
        throw std::invalid_argument("Invalid insert mode. Use ADD_VALUE or INSERT_VALUE.");

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::reset_system() -> void
{
    // check if the solver is initialized
    if (!this->_is_solver_initialized) {
        throw std::logic_error("Solver is not yet initialized. Call initialize() first.");
    }

    // fill the host matrix, rhs and solution with zeros
    _h_matrix.zero();
    this->_h_rhs.zero();
    this->_h_solution.zero();

    // reset the assembly finalized flag
    this->_is_assembly_finalized = false;

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::solve() -> void
{
    // check if the assembly is finalized
    if (!this->_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is not yet finalized. Call finalize_assembly() first.");
    }

    // copy the host matrix and rhs data to device global memory
    CHECK_CUDA_ERROR(cudaMemcpy(
        _d_matrix.data(), _h_matrix.data(), this->_size * this->_size * sizeof(real_type),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(
        this->_d_rhs.data(), this->_h_rhs.data(), this->_size * sizeof(real_type),
        cudaMemcpyHostToDevice));

    // allocate device memory for temporary variables in the factorization
    int * d_pivot = nullptr;
    int * d_info = nullptr;
    real_type * d_workspace = nullptr;
    int workspace_size = 0;

    // check the solver type is either LU or Cholesky
    if (this->_solver_type != SolverType::LU && this->_solver_type != SolverType::CHOLESKY) {
        throw std::invalid_argument(
            "Invalid solver type. Only LU and Cholesky solvers are supported in the CUDA dense "
            "solver.");
    }

    // get the workspace size for the factorization
    if (this->_solver_type == SolverType::LU) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::getrf_buffer_size(
                _cusolver_handle, this->_size, this->_size, _d_matrix.data(), this->_size,
                &workspace_size));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrf_buffer_size(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, _d_matrix.data(),
                this->_size, &workspace_size));
    }

    CHECK_CUDA_ERROR(cudaMalloc(&d_pivot, this->_size * sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_info, sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_workspace, workspace_size * sizeof(real_type)));

    // perform the factorization
    if (this->_solver_type == SolverType::LU) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::getrf(
                _cusolver_handle, this->_size, this->_size, _d_matrix.data(), this->_size,
                d_workspace, d_pivot, d_info));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrf(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, _d_matrix.data(),
                this->_size, d_workspace, workspace_size, d_info));
    }
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // solve the linear system
    if (this->_solver_type == SolverType::LU) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::getrs(
                _cusolver_handle, CUBLAS_OP_N, this->_size, 1, _d_matrix.data(), this->_size,
                d_pivot, this->_d_rhs.data(), this->_size, d_info));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrs(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, 1, _d_matrix.data(),
                this->_size, this->_d_rhs.data(), this->_size, d_info));
    }
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // copy the solution from device global memory to host memory
    // NOTE: _d_rhs contains the solution after the call to getrs/potrs as its contents are
    // overwritten by the solution vector
    CHECK_CUDA_ERROR(cudaMemcpy(
        this->_h_solution.data(), this->_d_rhs.data(), this->_size * sizeof(real_type),
        cudaMemcpyDeviceToHost));

    // free the temporary device memory
    CHECK_CUDA_ERROR(cudaFree(d_pivot));
    CHECK_CUDA_ERROR(cudaFree(d_info));
    CHECK_CUDA_ERROR(cudaFree(d_workspace));

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_initialize_cusolver() -> void
{
    // create the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnCreate(&_cusolver_handle));

    // create a cuda stream
    CHECK_CUDA_ERROR(cudaStreamCreateWithPriority(&(this->_cuda_stream), cudaStreamNonBlocking, 0));

    // set the stream for the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnSetStream(_cusolver_handle, this->_cuda_stream));

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_finalize_cusolver() -> void
{
    // destroy the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnDestroy(_cusolver_handle));

    // destroy the cuda stream
    CHECK_CUDA_ERROR(cudaStreamDestroy(this->_cuda_stream));

    // reset the handle and stream pointers
    _cusolver_handle = nullptr;
    this->_cuda_stream = nullptr;

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_allocate_host_memory(size_t size) -> void
{
    // allocate host memory for matrix, rhs and solution
    _h_matrix.allocate(size * size);
    this->_h_rhs.allocate(size);
    this->_h_solution.allocate(size);

    // zero out the arrays
    _h_matrix.zero();
    this->_h_rhs.zero();
    this->_h_solution.zero();

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_allocate_device_memory(size_t size) -> void
{
    // allocate global device memory for matrix and rhs
    _d_matrix.allocate(size * size);
    this->_d_rhs.allocate(size);

    // all done
    return;
}

// explicit instantiation of the {CUDADenseSolver} class for doubles
template class mito::solvers::cuda::CUDADenseSolver<double>;

// explicit instantiation of the {CUDADenseSolver} class for floats
template class mito::solvers::cuda::CUDADenseSolver<float>;
