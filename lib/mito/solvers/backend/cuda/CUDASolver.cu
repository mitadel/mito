// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDASolver<realT>::CUDASolver(SolverType solver_type) :
    _h_rhs(),
    _h_solution(),
    _d_rhs(),
    _solver_type(solver_type),
    _size(0),
    _is_solver_initialized(false),
    _is_assembly_finalized(false),
    _cuda_stream(nullptr)
{}

// destructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDASolver<realT>::~CUDASolver()
{}

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASolver<realT>::initialize(size_t size) -> void
{
    // check if the solver is already initialized
    if (_is_solver_initialized) {
        throw std::logic_error(
            "Solver is already initialized. You are not allowed to re-initialize it. If you want "
            "to re-initialize, you need to create a new solver instance.");
    }

    // check if the size is valid
    if (size <= 0) {
        throw std::invalid_argument("Size of the linear system must be greater than zero.");
    }

    // save the size of the linear system
    _size = size;

    // allocate host memory
    _allocate_host_memory(size);

    // allocate device memory
    _allocate_device_memory(size);

    // turn on the solver initialized flag
    _is_solver_initialized = true;

    // all done
    return;
}

// add/insert {value} to rhs entry at {row} of the host copy
template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASolver<realT>::set_rhs_value(
    size_t row, const real_type value, const InsertMode insert_mode) -> void
{
    // check if the system assembly is finalized and throw an error if it is
    if (_is_assembly_finalized) {
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
mito::solvers::cuda::CUDASolver<realT>::finalize_assembly() -> void
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

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASolver<realT>::_check_index_validity(size_t index) const -> void
{
    // check if the solver is initialized
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

// explicit instantiation of the {CUDASolver} class for doubles
template class mito::solvers::cuda::CUDASolver<double>;

// explicit instantiation of the {CUDASolver} class for floats
template class mito::solvers::cuda::CUDASolver<float>;
