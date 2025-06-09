// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// CUDA error checking macro
#define CHECK_CUDA_ERROR(call)                                                   \
    do {                                                                         \
        cudaError_t err = call;                                                  \
        if (err != cudaSuccess) {                                                \
            fprintf(stderr, "CUDA error at %s:%d\n", __FILE__, __LINE__);        \
            fprintf(stderr, "  Error: %s (%d)\n", cudaGetErrorString(err), err); \
            exit(EXIT_FAILURE);                                                  \
        }                                                                        \
    } while (0)

// function to convert cuSOLVER error codes to strings
inline const char *
cusolverGetErrorString(cusolverStatus_t status)
{
    switch (status) {
        case CUSOLVER_STATUS_SUCCESS:
            return "CUSOLVER_STATUS_SUCCESS";
        case CUSOLVER_STATUS_NOT_INITIALIZED:
            return "CUSOLVER_STATUS_NOT_INITIALIZED";
        case CUSOLVER_STATUS_ALLOC_FAILED:
            return "CUSOLVER_STATUS_ALLOC_FAILED";
        case CUSOLVER_STATUS_INVALID_VALUE:
            return "CUSOLVER_STATUS_INVALID_VALUE";
        case CUSOLVER_STATUS_ARCH_MISMATCH:
            return "CUSOLVER_STATUS_ARCH_MISMATCH";
        case CUSOLVER_STATUS_MAPPING_ERROR:
            return "CUSOLVER_STATUS_MAPPING_ERROR";
        case CUSOLVER_STATUS_EXECUTION_FAILED:
            return "CUSOLVER_STATUS_EXECUTION_FAILED";
        case CUSOLVER_STATUS_INTERNAL_ERROR:
            return "CUSOLVER_STATUS_INTERNAL_ERROR";
        case CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
            return "CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED";
        default:
            return "Unknown cuSOLVER error";
    }
}

// cuSOLVER error checking macro
#define CHECK_CUSOLVER_ERROR(call)                                                                 \
    do {                                                                                           \
        cusolverStatus_t status = (call);                                                          \
        if (status != CUSOLVER_STATUS_SUCCESS) {                                                   \
            fprintf(                                                                               \
                stderr, "cuSOLVER error: %s at %s:%d\n", cusolverGetErrorString(status), __FILE__, \
                __LINE__);                                                                         \
            exit(EXIT_FAILURE);                                                                    \
        }                                                                                          \
    } while (0)

// constructor
mito::solvers::cuda::CUDADenseSolver::CUDADenseSolver() :
    _h_matrix(nullptr),
    _h_rhs(nullptr),
    _h_solution(nullptr),
    _d_matrix(nullptr),
    _d_rhs(nullptr),
    _size(0),
    _is_solver_initialized(false),
    _allocated_host_memory_type(0),
    _is_assembly_finalized(false),
    _cusolver_handle(nullptr),
    _cuda_stream(nullptr)
{
    // initialize cuSOLVER
    _initialize_cusolver();
}

// destructor
mito::solvers::cuda::CUDADenseSolver::~CUDADenseSolver()
{
    // finalize cuSOLVER
    _finalize_cusolver();
}

auto
mito::solvers::cuda::CUDADenseSolver::initialize(size_t size) -> void
{
    // check if the solver is already initialized
    if (_is_solver_initialized) {
        throw std::logic_error(
            "Solver is already initialized. Are you sure you want to reinitialize? Then call "
            "finalize() first.");
    }

    // check if the size is valid
    if (size <= 0) {
        throw std::invalid_argument("Size of the linear system must be greater than zero.");
    }

    // save the size of the linear system
    _size = size;

    // allocate host memory
    _allocate_host_memory(size);

    // initialize host data
    _initialize_host_data(size);

    // allocate device memory
    _allocate_device_memory(size);

    // turn on the solver initialized flag
    _is_solver_initialized = true;

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::finalize() -> void
{
    // check if the solver is initialized
    if (_is_solver_initialized) {
        // free host memory
        _free_host_memory();

        // free device memory
        _free_device_memory();
    }

    // reset the solver initialized flag
    _is_solver_initialized = false;

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::reset_system() -> void
{
    // check if the solver is initialized
    if (!_is_solver_initialized) {
        throw std::logic_error("Solver is not yet initialized. Call initialize() first.");
    }

    // fill the host matrix, rhs and solution with zeros
    _initialize_host_data(_size);

    // reset the assembly finalized flag
    _is_assembly_finalized = false;

    // all done
    return;
}

// add/insert {value} to matrix entry at ({row}, {col}) of the host copy
auto
mito::solvers::cuda::CUDADenseSolver::set_matrix_value(
    size_t row, size_t col, const double value,
    const mito::solvers::cuda::InsertMode insert_mode = mito::solvers::cuda::InsertMode::ADD_VALUE)
    -> void
{
    // check if the system assembly is finalized and throw an error if it is
    if (_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is already finalized. Cannot add/insert values to the matrix.");
    }

    // check if the row and column indices are within bounds
    _check_index_validity(row);
    _check_index_validity(col);

    // add/insert the value to the matrix entry in the host matrix
    if (insert_mode == mito::solvers::cuda::InsertMode::ADD_VALUE)
        _h_matrix[row * _size + col] += value;
    else if (insert_mode == mito::solvers::cuda::InsertMode::INSERT_VALUE)
        _h_matrix[row * _size + col] = value;
    else
        throw std::invalid_argument("Invalid insert mode. Use ADD_VALUE or INSERT_VALUE.");

    // all done
    return;
}

// add/insert {value} to rhs entry at {row} of the host copy
auto
mito::solvers::cuda::CUDADenseSolver::set_rhs_value(
    size_t row, const double value,
    const mito::solvers::cuda::InsertMode insert_mode = mito::solvers::cuda::InsertMode::ADD_VALUE)
    -> void
{
    // check if the system assembly is finalized and throw an error if it is
    if (_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is already finalized. Cannot add/insert values to the rhs.");
    }

    // check if the row index is within bounds
    _check_index_validity(row);

    // add/insert the value to the rhs entry in the host rhs
    if (insert_mode == mito::solvers::cuda::InsertMode::ADD_VALUE)
        _h_rhs[row] += value;
    else if (insert_mode == mito::solvers::cuda::InsertMode::INSERT_VALUE)
        _h_rhs[row] = value;
    else
        throw std::invalid_argument("Invalid insert mode. Use ADD_VALUE or INSERT_VALUE.");

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::finalize_assembly() -> void
{
    // check if the solver is initialized
    if (!_is_solver_initialized) {
        throw std::logic_error(
            "Solver is not yet initialized. Call initialize() first, assemble the "
            "system, and then finalize the assembly.");
    }

    // issue a warning that all entries should be set before finalizing the assembly
    std::cerr
        << "Warning: Finalizing assembly. Make sure all system entries are set before this step."
        << std::endl;

    // set the assembly finalized flag to true
    _is_assembly_finalized = true;

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::solve() -> void
{
    // check if the assembly is finalized
    if (!_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is not yet finalized. Call finalize_assembly() first.");
    }

    // copy the host matrix and rhs data to device global memory
    // IMPROVE: We should move the data through streams for better performance later!
    CHECK_CUDA_ERROR(
        cudaMemcpy(_d_matrix, _h_matrix, _size * _size * sizeof(double), cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(_d_rhs, _h_rhs, _size * sizeof(double), cudaMemcpyHostToDevice));

    // allocate device memory for temporary variables in the LU factorization
    int * d_pivot = nullptr;
    int * d_info = nullptr;
    double * d_workspace = nullptr;
    int workspace_size = 0;

    // get the workspace size for getrf (only double precision LU factorization is supported!)
    // QUESTION: Should we check if mito::real is double or float and allocate the workspace memory
    // accordingly?
    CHECK_CUSOLVER_ERROR(cusolverDnDgetrf_bufferSize(
        _cusolver_handle, _size, _size, _d_matrix, _size, &workspace_size));

    CHECK_CUDA_ERROR(cudaMalloc(&d_pivot, _size * sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_info, sizeof(int)));
    CHECK_CUDA_ERROR(cudaMalloc(&d_workspace, workspace_size * sizeof(double)));

    // perform LU factorization
    CHECK_CUSOLVER_ERROR(cusolverDnDgetrf(
        _cusolver_handle, _size, _size, _d_matrix, _size, d_workspace, d_pivot, d_info));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // solve the linear system
    CHECK_CUSOLVER_ERROR(cusolverDnDgetrs(
        _cusolver_handle, CUBLAS_OP_N, _size, 1, _d_matrix, _size, d_pivot, _d_rhs, _size, d_info));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // copy the solution from device global memory to host memory
    // NOTE: _d_rhs contains the solution after the call to getrs as its contents are overwritten
    // by the solution vector
    CHECK_CUDA_ERROR(
        cudaMemcpy(_h_solution, _d_rhs, _size * sizeof(double), cudaMemcpyDeviceToHost));

    // free the temporary device memory
    CHECK_CUDA_ERROR(cudaFree(d_pivot));
    CHECK_CUDA_ERROR(cudaFree(d_info));
    CHECK_CUDA_ERROR(cudaFree(d_workspace));

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::_initialize_cusolver() -> void
{
    // create the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnCreate(&_cusolver_handle));

    // create a cuda stream
    CHECK_CUDA_ERROR(cudaStreamCreateWithPriority(&_cuda_stream, cudaStreamNonBlocking, 0));

    // set the stream for the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnSetStream(_cusolver_handle, _cuda_stream));

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::_finalize_cusolver() -> void
{
    // destroy the cuSOLVER handle
    CHECK_CUSOLVER_ERROR(cusolverDnDestroy(_cusolver_handle));

    // destroy the cuda stream
    CHECK_CUDA_ERROR(cudaStreamDestroy(_cuda_stream));

    // reset the handle and stream pointers
    _cusolver_handle = nullptr;
    _cuda_stream = nullptr;

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::_allocate_host_memory(size_t size) -> void
{
    // try to allocate pinned memory on the host for faster transfers
    cudaError_t err_pinned_alloc_matrix = cudaMallocHost(&_h_matrix, size * size * sizeof(double));
    cudaError_t err_pinned_alloc_rhs = cudaMallocHost(&_h_rhs, size * sizeof(double));
    cudaError_t err_pinned_alloc_solution = cudaMallocHost(&_h_solution, size * sizeof(double));

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
        _h_matrix = new double[size * size];
        _h_rhs = new double[size];
        _h_solution = new double[size];
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

auto
mito::solvers::cuda::CUDADenseSolver::_allocate_device_memory(size_t size) -> void
{
    // allocate global device memory for matrix, rhs, and solution
    CHECK_CUDA_ERROR(cudaMalloc(&_d_matrix, size * size * sizeof(double)));
    CHECK_CUDA_ERROR(cudaMalloc(&_d_rhs, size * sizeof(double)));

    // all done
    return;
}

auto
mito::solvers::cuda::CUDADenseSolver::_initialize_host_data(size_t size) -> void
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

auto
mito::solvers::cuda::CUDADenseSolver::_free_host_memory() -> void
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

auto
mito::solvers::cuda::CUDADenseSolver::_free_device_memory() -> void
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

auto
mito::solvers::cuda::CUDADenseSolver::_check_index_validity(size_t index) const -> void
{
    // check if the solver is initialized
    // QUESTION: checking multiple times for initialization may be inefficient?
    if (!_is_solver_initialized) {
        throw std::logic_error("Solver is not yet initialized. Call initialize() first.");
    }

    // check if the index is valid and return false if it is not
    if (index >= _size) {
        throw std::out_of_range(
            "Index " + std::to_string(index) + " is out of range. It must be between 0 and "
            + std::to_string(_size - 1) + ".");
    }

    // all done
    return;
}