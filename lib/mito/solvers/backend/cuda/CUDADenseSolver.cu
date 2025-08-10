// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDADenseSolver<realT>::CUDADenseSolver(SolverType solver_type) :
    CUDASolver<realT>(solver_type),
    _h_matrix(nullptr),
    _h_rhs(nullptr),
    _h_solution(nullptr),
    _d_matrix(nullptr),
    _d_rhs(nullptr),
    _allocated_host_memory_type(0),
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

// add/insert {value} to rhs entry at {row} of the host copy
template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::set_rhs_value(
    size_t row, const real_type value, const InsertMode insert_mode) -> void
{
    // check if the system assembly is finalized and throw an error if it is
    if (this->_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is already finalized. Cannot add/insert values to the rhs.");
    }

    // check if the row index is within bounds
    this->_check_index_validity(row);

    // add/insert the value to the rhs entry in the host rhs
    if (insert_mode == InsertMode::ADD_VALUE)
        _h_rhs[row] += value;
    else if (insert_mode == InsertMode::INSERT_VALUE)
        _h_rhs[row] = value;
    else
        throw std::invalid_argument("Invalid insert mode. Use ADD_VALUE or INSERT_VALUE.");

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
    // IMPROVE: We should move the data through streams for better performance later!
    CHECK_CUDA_ERROR(cudaMemcpy(
        _d_matrix, _h_matrix, this->_size * this->_size * sizeof(real_type),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(
        cudaMemcpy(_d_rhs, _h_rhs, this->_size * sizeof(real_type), cudaMemcpyHostToDevice));

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
                _cusolver_handle, this->_size, this->_size, _d_matrix, this->_size,
                &workspace_size));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrf_buffer_size(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, _d_matrix, this->_size,
                &workspace_size));
    }

    CHECK_CUDA_ERROR(cudaMalloc(&d_pivot, this->_size * sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_info, sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_workspace, workspace_size * sizeof(real_type)));

    // perform the factorization
    if (this->_solver_type == SolverType::LU) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::getrf(
                _cusolver_handle, this->_size, this->_size, _d_matrix, this->_size, d_workspace,
                d_pivot, d_info));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrf(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, _d_matrix, this->_size,
                d_workspace, workspace_size, d_info));
    }
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // solve the linear system
    if (this->_solver_type == SolverType::LU) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::getrs(
                _cusolver_handle, CUBLAS_OP_N, this->_size, 1, _d_matrix, this->_size, d_pivot,
                _d_rhs, this->_size, d_info));
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        CHECK_CUSOLVER_ERROR(
            cusolver_traits<real_type>::potrs(
                _cusolver_handle, CUBLAS_FILL_MODE_LOWER, this->_size, 1, _d_matrix, this->_size,
                _d_rhs, this->_size, d_info));
    }
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // copy the solution from device global memory to host memory
    // NOTE: _d_rhs contains the solution after the call to getrs/potrs as its contents are
    // overwritten by the solution vector
    CHECK_CUDA_ERROR(
        cudaMemcpy(_h_solution, _d_rhs, this->_size * sizeof(real_type), cudaMemcpyDeviceToHost));

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
    // try to allocate pinned memory on the host for faster transfers
    cudaError_t err_pinned_alloc_matrix =
        cudaMallocHost(&_h_matrix, size * size * sizeof(real_type));
    cudaError_t err_pinned_alloc_rhs = cudaMallocHost(&_h_rhs, size * sizeof(real_type));
    cudaError_t err_pinned_alloc_solution = cudaMallocHost(&_h_solution, size * sizeof(real_type));

    // check if the pinned memory allocation for matrix, rhs, and solution was successful
    if (err_pinned_alloc_matrix == cudaSuccess && err_pinned_alloc_rhs == cudaSuccess
        && err_pinned_alloc_solution == cudaSuccess) {
        // set the flag to indicate that pinned memory was allocated
        _allocated_host_memory_type = 1;
        return;
    }

    // free any partially allocated pinned memory
    if (err_pinned_alloc_matrix == cudaSuccess)
        CHECK_CUDA_ERROR(cudaFreeHost(_h_matrix));
    if (err_pinned_alloc_rhs == cudaSuccess)
        CHECK_CUDA_ERROR(cudaFreeHost(_h_rhs));
    if (err_pinned_alloc_solution == cudaSuccess)
        CHECK_CUDA_ERROR(cudaFreeHost(_h_solution));

    // try to allocate regular memory on the host
    try {
        _h_matrix = new real_type[size * size];
        _h_rhs = new real_type[size];
        _h_solution = new real_type[size];
        // set the flag to indicate that regular memory was allocated
        _allocated_host_memory_type = 2;
    } catch (const std::bad_alloc & e) {
        throw std::runtime_error(
            "Failed to allocate host memory for matrix, rhs, and solution: "
            + std::string(e.what()));
    }

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_allocate_device_memory(size_t size) -> void
{
    // allocate global device memory for matrix, rhs, and solution
    CHECK_CUDA_ERROR(cudaMalloc(&_d_matrix, size * size * sizeof(real_type)));
    CHECK_CUDA_ERROR(cudaMalloc(&_d_rhs, size * sizeof(real_type)));

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_initialize_host_data(size_t size) -> void
{
    // check if host memory is allocated
    if (_allocated_host_memory_type == 0) {
        // throw developer error
        throw std::logic_error(
            "Host memory is not yet allocated. Call _allocate_host_memory() first.");
    }

    // initialize the host matrix, rhs and solution with zeros
    for (size_t i = 0; i < size * size; ++i) {
        _h_matrix[i] = 0.0;
    }
    for (size_t i = 0; i < size; ++i) {
        _h_rhs[i] = 0.0;
        _h_solution[i] = 0.0;
    }

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_free_host_memory() -> void
{
    // check if pinned memory was allocated
    if (_allocated_host_memory_type == 1) {
        // free pinned memory
        if (_h_matrix)
            CHECK_CUDA_ERROR(cudaFreeHost(_h_matrix));
        if (_h_rhs)
            CHECK_CUDA_ERROR(cudaFreeHost(_h_rhs));
        if (_h_solution)
            CHECK_CUDA_ERROR(cudaFreeHost(_h_solution));
    } else if (_allocated_host_memory_type == 2) {
        // free regular memory
        if (_h_matrix)
            delete[] _h_matrix;
        if (_h_rhs)
            delete[] _h_rhs;
        if (_h_solution)
            delete[] _h_solution;
    }

    // reset the flag to indicate that the memory has been freed
    _allocated_host_memory_type = 0;
    // reset the pointers
    _h_matrix = nullptr;
    _h_rhs = nullptr;
    _h_solution = nullptr;

    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDADenseSolver<realT>::_free_device_memory() -> void
{
    // free global device memory for matrix and rhs
    if (_d_matrix)
        CHECK_CUDA_ERROR(cudaFree(_d_matrix));
    if (_d_rhs)
        CHECK_CUDA_ERROR(cudaFree(_d_rhs));

    // reset the pointers
    _d_matrix = nullptr;
    _d_rhs = nullptr;

    return;
}

// explicit instantiation of the {CUDADenseSolver} class for doubles
template class mito::solvers::cuda::CUDADenseSolver<double>;

// explicit instantiation of the {CUDADenseSolver} class for floats
template class mito::solvers::cuda::CUDADenseSolver<float>;
