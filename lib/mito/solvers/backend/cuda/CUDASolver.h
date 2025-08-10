// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    template <real_c realT>
    class CUDASolver {
      public:
        // type alias for real numbers
        using real_type = realT;

      public:
        // constructor
        CUDASolver(SolverType solver_type);

        // destructor
        ~CUDASolver();

      public:
        // initialize the CUDA dense solver
        auto initialize(size_t size) -> void;

        // finalize the CUDA dense solver
        auto finalize() -> void;

        // reset the linear system (i.e. the host copy of the matrix, right-hand side and solution)
        auto reset_system() -> void;

        // set (add or insert depending on the mode) the value of a matrix entry in the host copy
        virtual auto set_matrix_value(
            size_t row, size_t col, const real_type value,
            const InsertMode insert_mode = InsertMode::INSERT_VALUE) -> void = 0;

        // set (add or insert depending on the mode) the value of a right-hand side entry in the
        // host copy
        virtual auto set_rhs_value(
            size_t row, const real_type value,
            const InsertMode insert_mode = InsertMode::INSERT_VALUE) -> void = 0;

        // finalize the linear system assembly
        auto finalize_assembly() -> void;

        // solve the linear system
        virtual auto solve() -> void = 0;

        // get the solution vector
        virtual auto get_solution(std::vector<real_type> & solution) const -> void = 0;

      protected:
        // initialize the cuSOLVER utilities
        virtual auto _initialize_cusolver() -> void = 0;

        // destroy the cuSOLVER utilities
        virtual auto _finalize_cusolver() -> void = 0;

        // allocate the host memory for the matrix, right-hand side, and solution
        virtual auto _allocate_host_memory(size_t size) -> void = 0;

        // allocate the device memory for the matrix and right-hand side
        virtual auto _allocate_device_memory(size_t size) -> void = 0;

        // initialize the host data for the matrix, right-hand side, and solution
        virtual auto _initialize_host_data(size_t size) -> void = 0;

        // deallocate the host memory for the matrix, right-hand side, and solution
        virtual auto _free_host_memory() -> void = 0;

        // deallocate the device memory for the matrix and right-hand side
        virtual auto _free_device_memory() -> void = 0;

        // check the validity of the index in the matrix and right-hand side
        auto _check_index_validity(size_t index) const -> void;

      protected:
        // solver type
        SolverType _solver_type;
        // size of the linear system
        size_t _size;
        // flag to indicate if the solver has been initialized
        bool _is_solver_initialized;
        // flag to indicate if the system assembly has been finalized
        bool _is_assembly_finalized;
        // cuda stream
        cudaStream_t _cuda_stream;
    };
}

// end of file
