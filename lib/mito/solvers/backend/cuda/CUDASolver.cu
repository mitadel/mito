// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
template <mito::solvers::cuda::real_c realT>
mito::solvers::cuda::CUDASolver<realT>::CUDASolver(SolverType solver_type) :
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

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASolver<realT>::finalize() -> void
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

template <mito::solvers::cuda::real_c realT>
auto
mito::solvers::cuda::CUDASolver<realT>::reset_system() -> void
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
