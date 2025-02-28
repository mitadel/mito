// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "public.h"


// constructor
mito::solvers::petsc::PETScKrylovSolver::PETScKrylovSolver() : _initialized_petsc(false) {}

// destructor
mito::solvers::petsc::PETScKrylovSolver::~PETScKrylovSolver() {}

// initialize PETSc if it has not been initialized before
auto
mito::solvers::petsc::PETScKrylovSolver::_initialize_petsc() -> void
{
    // check if PETSc has been initialized
    PetscBool petsc_already_initialized;
    PetscCallVoid(PetscInitialized(&petsc_already_initialized));

    // if PETSc has not been initialized before
    if (petsc_already_initialized == PETSC_FALSE) {
        // initialize PETSc
        PetscCallVoid(PetscInitializeNoArguments());
        // and remember that you have initialized PETSc
        _initialized_petsc = true;
    }

    // all done
    return;
}

// finalize PETSc if it has been initialized by this instance
auto
mito::solvers::petsc::PETScKrylovSolver::_finalize_petsc() -> void
{
    // if PETSc was initialized by this instance
    if (_initialized_petsc) {
        // finalize PETSc
        PetscCallVoid(PetscFinalize());
    }

    // all done
    return;
}

auto
mito::solvers::petsc::PETScKrylovSolver::_create_linear_system(index_type size) -> void
{
    // create the vectors
    PetscCallVoid(VecCreate(PETSC_COMM_WORLD, &_solution));
    PetscCallVoid(VecSetSizes(_solution, PETSC_DECIDE, size));
    PetscCallVoid(VecCreate(PETSC_COMM_WORLD, &_rhs));
    PetscCallVoid(VecSetSizes(_rhs, PETSC_DECIDE, size));

    // create the matrix
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &_matrix));
    PetscCallVoid(MatSetSizes(_matrix, PETSC_DECIDE, PETSC_DECIDE, size, size));

    // create the Krylov solver
    PetscCallVoid(KSPCreate(PETSC_COMM_WORLD, &_ksp));
    PetscCallVoid(KSPSetOperators(_ksp, _matrix, _matrix));

    // set the default options (do not allow the user to control the options for matrix and vectors)
    PetscCallVoid(MatSetFromOptions(_matrix));
    PetscCallVoid(VecSetFromOptions(_rhs));
    PetscCallVoid(VecSetFromOptions(_solution));

    // all done
    return;
}

auto
mito::solvers::petsc::PETScKrylovSolver::_destroy_linear_system() -> void
{
    // destroy  the matrix, right-hand side, solution, and Krylov solver
    PetscCallVoid(MatDestroy(&_matrix));
    PetscCallVoid(VecDestroy(&_solution));
    PetscCallVoid(VecDestroy(&_rhs));
    PetscCallVoid(KSPDestroy(&_ksp));

    // all done
    return;
}

// initialize the petsc solver
auto
mito::solvers::petsc::PETScKrylovSolver::initialize(index_type size) -> void
{
    // initialize PETSc if it has not been initialized before
    _initialize_petsc();

    // create the matrix, right-hand side, solution, and Krylov solver
    _create_linear_system(size);

    // all done
    return;
}

// finalize the petsc solver
auto
mito::solvers::petsc::PETScKrylovSolver::finalize() -> void
{
    // destroy the memory for the matrix, right-hand side, solution, and Krylov solver
    _destroy_linear_system();

    // finalize petsc if it has been initialized by this instance
    _finalize_petsc();

    // all done
    return;
}

// set petsc options
auto
mito::solvers::petsc::PETScKrylovSolver::set_options(const options_type & options) -> void
{
    // the number of arguments
    PetscInt petsc_argc = std::size(options);

    // populate the arguments list
    std::vector<char *> options_chars(petsc_argc);
    for (const auto & option : options) {
        // add the option to the list of arguments
        options_chars.push_back(const_cast<char *>(option.c_str()));
    }
    auto petsc_argv = options_chars.data();

    // record the options with PETSc
    PetscCallVoid(PetscOptionsInsert(PETSC_NULLPTR, &petsc_argc, &petsc_argv, PETSC_NULLPTR));

    // TODO: possibly add an options prefix to avoid conflicts when multiple solvers are used
    PetscCallVoid(KSPSetFromOptions(_ksp));

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::solvers::petsc::PETScKrylovSolver::insert_matrix_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::solvers::petsc::PETScKrylovSolver::add_matrix_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// set the right-hand side entry at {row} to {value}
auto
mito::solvers::petsc::PETScKrylovSolver::insert_rhs_value(index_type row, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to right-hand side entry at {row}
auto
mito::solvers::petsc::PETScKrylovSolver::add_rhs_value(index_type row, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, ADD_VALUES));

    // all done
    return;
}

// solve the linear system
auto
mito::solvers::petsc::PETScKrylovSolver::solve() -> void
{

    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // // show the matrix and the right-hand-side
    // PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));
    // PetscCallVoid(VecView(_rhs, PETSC_VIEWER_STDOUT_WORLD));

    // solve the linear system
    PetscCallVoid(KSPSolve(_ksp, _rhs, _solution));

    // // show the solution
    // PetscCallVoid(VecView(_solution, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}


// end of file
