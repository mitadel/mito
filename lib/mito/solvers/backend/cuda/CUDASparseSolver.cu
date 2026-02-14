// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDASparseSolver<realT>::CUDASparseSolver(SolverType solver_type) :
    CUDASolver<realT>(solver_type),
    _h_matrix(),
    _d_matrix(),
    _d_solution(),
    _cudss_handle()
{
    // initialize cuSOLVER
    _initialize_cusolver();
}

// destructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDASparseSolver<realT>::~CUDASparseSolver()
{
    // finalize cuSOLVER
    _finalize_cusolver();
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::initialize(size_t size, size_t nnz) -> void
{
    // call the base class initialize function
    CUDASolver<realT>::initialize(size);

    // reserve space for non-zero entries per row in the host sparse matrix for efficiency
    // NOTE: This is just an estimate. The actual number of non-zero entries may be less than
    // nnz. But it is better to set it to a reasonable value to avoid multiple reallocations.
    // In general, over-allocating is better than under-allocating.
    (nnz == 0) ? _h_matrix.reserve(Eigen::VectorXi::Constant(size, size)) :
                 _h_matrix.reserve(Eigen::VectorXi::Constant(size, nnz));

    // all done
    return;
}

// add/insert {value} to matrix entry at ({row}, {col}) of the host copy
template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::set_matrix_value(
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
    // NOTE: We store the host matrix in CSR format using Eigen's SparseMatrix class in row-major
    // order.
    if (insert_mode == InsertMode::ADD_VALUE)
        _h_matrix.coeffRef(row, col) += value;
    else if (insert_mode == InsertMode::INSERT_VALUE)
        _h_matrix.coeffRef(row, col) = value;
    else
        throw std::invalid_argument("Invalid insert mode. Use ADD_VALUE or INSERT_VALUE.");

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::reset_system() -> void
{
    // check if the solver is initialized
    if (!this->_is_solver_initialized) {
        throw std::logic_error("Solver is not yet initialized. Call initialize() first.");
    }

    // fill the host matrix, rhs and solution with zeros
    _h_matrix.setZero();
    this->_h_rhs.zero();
    this->_h_solution.zero();

    // reset the assembly finalized flag
    this->_is_assembly_finalized = false;

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::solve() -> void
{
    // check if the assembly is finalized
    if (!this->_is_assembly_finalized) {
        throw std::logic_error(
            "System assembly is not yet finalized. Call finalize_assembly() first.");
    }

    // finalize the host matrix (compress the storage)
    // NOTE: This is not a mandatory step as we are only using the Eigen matrix for storage on the
    // host but we are doing it anyway.
    _h_matrix.makeCompressed();

    // create and fill the auxiliary containers to hold the CSR representation of the host matrix
    size_t nnz_matrix = _h_matrix.nonZeros();
    std::vector<real_type> h_csr_values(nnz_matrix, 0.0);
    std::vector<int> h_csr_row_offsets(this->_size + 1, 0);
    std::vector<int> h_csr_col_indices(nnz_matrix, 0);
    int idx = 0;
    for (size_t k = 0; k < this->_size; ++k) {    // for each row
        h_csr_row_offsets[k] = idx;
        for (typename Eigen::SparseMatrix<real_type, Eigen::RowMajor>::InnerIterator it(
                 _h_matrix, k);
             it; ++it) {    // for each non-zero entry in the row
            h_csr_values[idx] = it.value();
            h_csr_col_indices[idx] = it.col();
            ++idx;
        }
    }
    h_csr_row_offsets[this->_size] = nnz_matrix;

    // allocate device memory for the matrix
    _d_matrix.allocate(this->_size, this->_size, nnz_matrix);

    // copy the host matrix and rhs data to device global memory
    CHECK_CUDA_ERROR(cudaMemcpy(
        _d_matrix.values(), h_csr_values.data(), h_csr_values.size() * sizeof(real_type),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(
        _d_matrix.row_offsets(), h_csr_row_offsets.data(), h_csr_row_offsets.size() * sizeof(int),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(
        _d_matrix.col_indices(), h_csr_col_indices.data(), h_csr_col_indices.size() * sizeof(int),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(
        this->_d_rhs.data(), this->_h_rhs.data(), this->_size * sizeof(real_type),
        cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // cuDSS data structures to solve the linear system
    cudssConfig_t config;
    cudssData_t data;
    cudssMatrix_t d_A, d_b, d_x;

    // create the cuDSS config and data structures
    CHECK_CUDSS_ERROR(cudssConfigCreate(&config));
    CHECK_CUDSS_ERROR(cudssDataCreate(_cudss_handle, &data));

    // determine the data type
    cudaDataType_t dtype;
    if constexpr (std::is_same_v<real_type, double>) {
        dtype = CUDA_R_64F;
    } else if constexpr (std::is_same_v<real_type, float>) {
        dtype = CUDA_R_32F;
    }

    // determine the matrix type based on the solver type
    cudssMatrixType_t mtype;
    if (this->_solver_type == SolverType::LU) {
        mtype = CUDSS_MTYPE_GENERAL;
    } else if (this->_solver_type == SolverType::CHOLESKY) {
        mtype = CUDSS_MTYPE_SPD;
    } else {
        throw std::logic_error(
            "Unsupported solver type. Only LU and CHOLESKY are supported in the CUDA sparse "
            "solver.");
    }

    // create (CSR) matrix and vector descriptors
    CHECK_CUDSS_ERROR(cudssMatrixCreateCsr(
        &d_A, this->_size, this->_size, nnz_matrix, _d_matrix.row_offsets(), NULL,
        _d_matrix.col_indices(), _d_matrix.values(), CUDA_R_32I, dtype, mtype, CUDSS_MVIEW_FULL,
        CUDSS_BASE_ZERO));
    CHECK_CUDSS_ERROR(cudssMatrixCreateDn(
        &d_b, this->_size, 1, this->_size, this->_d_rhs.data(), dtype, CUDSS_LAYOUT_COL_MAJOR));
    CHECK_CUDSS_ERROR(cudssMatrixCreateDn(
        &d_x, this->_size, 1, this->_size, _d_solution.data(), dtype, CUDSS_LAYOUT_COL_MAJOR));

    /************* solve the linear system in three stages *************/

    // Stage 1: reordering & symbolic factorization
    CHECK_CUDSS_ERROR(
        cudssExecute(_cudss_handle, CUDSS_PHASE_ANALYSIS, config, data, d_A, d_x, d_b));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // Stage 2: numerical factorization
    CHECK_CUDSS_ERROR(
        cudssExecute(_cudss_handle, CUDSS_PHASE_FACTORIZATION, config, data, d_A, d_x, d_b));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // Stage 3: solve the system
    CHECK_CUDSS_ERROR(cudssExecute(_cudss_handle, CUDSS_PHASE_SOLVE, config, data, d_A, d_x, d_b));
    CHECK_CUDA_ERROR(cudaDeviceSynchronize());

    // copy the solution back to host
    CHECK_CUDA_ERROR(cudaMemcpy(
        this->_h_solution.data(), _d_solution.data(), this->_size * sizeof(real_type),
        cudaMemcpyDeviceToHost));

    // destroy the cuDSS data structures
    CHECK_CUDSS_ERROR(cudssMatrixDestroy(d_A));
    CHECK_CUDSS_ERROR(cudssMatrixDestroy(d_b));
    CHECK_CUDSS_ERROR(cudssMatrixDestroy(d_x));
    CHECK_CUDSS_ERROR(cudssDataDestroy(_cudss_handle, data));
    CHECK_CUDSS_ERROR(cudssConfigDestroy(config));

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::_initialize_cusolver() -> void
{
    // create the cuDSS handle
    CHECK_CUDSS_ERROR(cudssCreate(&_cudss_handle));

    // create a cuda stream
    CHECK_CUDA_ERROR(cudaStreamCreateWithPriority(&(this->_cuda_stream), cudaStreamNonBlocking, 0));

    // set the stream for the cuDSS handle
    CHECK_CUDSS_ERROR(cudssSetStream(_cudss_handle, this->_cuda_stream));

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::_finalize_cusolver() -> void
{
    // destroy the cuDSS handle
    CHECK_CUDSS_ERROR(cudssDestroy(_cudss_handle));

    // destroy the cuda stream
    CHECK_CUDA_ERROR(cudaStreamDestroy(this->_cuda_stream));

    // reset the handle and stream pointers
    _cudss_handle = nullptr;
    this->_cuda_stream = nullptr;

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::_allocate_host_memory(size_t size) -> void
{
    // allocate host memory for matrix, rhs and solution
    _h_matrix.resize(size, size);
    this->_h_rhs.allocate(size);
    this->_h_solution.allocate(size);

    // zero out the arrays
    _h_matrix.setZero();
    this->_h_rhs.zero();
    this->_h_solution.zero();

    // all done
    return;
}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASparseSolver<realT>::_allocate_device_memory(size_t size) -> void
{
    // NOTE: We are not allocating memory for the matrix here because we do not know the number of
    // non-zero entries yet and allocating device memory is an expensive operation. So we will
    // allocate the device memory for the matrix directly after the assembly is finalized.

    // allocate device memory for rhs and solution
    this->_d_rhs.allocate(size);
    _d_solution.allocate(size);

    // zero out the solution array
    _d_solution.zero();

    // all done
    return;
}

// explicit instantiation of the {CUDASparseSolver} class for doubles
template class mito::solvers::cuda::CUDASparseSolver<double>;

// explicit instantiation of the {CUDASparseSolver} class for floats
template class mito::solvers::cuda::CUDASparseSolver<float>;
