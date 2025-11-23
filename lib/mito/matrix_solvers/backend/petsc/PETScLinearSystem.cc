// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "PETScLinearSystem.h"


// constructor
mito::matrix_solvers::petsc::PETScLinearSystem::PETScLinearSystem(const label_type & label) :
    _label(label),
    _n_equations(0)
{}

// destructor
mito::matrix_solvers::petsc::PETScLinearSystem::~PETScLinearSystem() {}

// allocate memory for the matrix, right-hand side, and solution
auto
mito::matrix_solvers::petsc::PETScLinearSystem::create(index_type size) -> void
{
    // take note of the number of equations
    _n_equations = size;

    // create the vectors
    PetscCallVoid(VecCreate(PETSC_COMM_WORLD, &_solution));
    PetscCallVoid(VecSetSizes(_solution, PETSC_DECIDE, size));
    PetscCallVoid(VecCreate(PETSC_COMM_WORLD, &_rhs));
    PetscCallVoid(VecSetSizes(_rhs, PETSC_DECIDE, size));

    // create the matrix
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &_matrix));
    PetscCallVoid(MatSetSizes(_matrix, PETSC_DECIDE, PETSC_DECIDE, size, size));

    // set the default options (do not allow the user to control the options for matrix and
    // vectors)
    PetscCallVoid(MatSetFromOptions(_matrix));
    PetscCallVoid(VecSetFromOptions(_rhs));
    PetscCallVoid(VecSetFromOptions(_solution));

    // all done
    return;
}

// free memory for the matrix, right-hand side, and solution
auto
mito::matrix_solvers::petsc::PETScLinearSystem::destroy() -> void
{
    // destroy the matrix, right-hand side, solution
    PetscCallVoid(MatDestroy(&_matrix));
    PetscCallVoid(VecDestroy(&_solution));
    PetscCallVoid(VecDestroy(&_rhs));

    // all done
    return;
}

// get the label of the linear system
auto
mito::matrix_solvers::petsc::PETScLinearSystem::label() const -> label_type
{
    // easy enough
    return _label;
}

// assemble the linear system
auto
mito::matrix_solvers::petsc::PETScLinearSystem::assemble() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // // show the matrix and the right-hand-side
    // PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));
    // PetscCallVoid(VecView(_rhs, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::matrix_solvers::petsc::PETScLinearSystem::insert_matrix_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::matrix_solvers::petsc::PETScLinearSystem::add_matrix_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// set the right-hand side entry at {row} to {value}
auto
mito::matrix_solvers::petsc::PETScLinearSystem::insert_rhs_value(
    index_type row, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to right-hand side entry at {row}
auto
mito::matrix_solvers::petsc::PETScLinearSystem::add_rhs_value(
    index_type row, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, ADD_VALUES));

    // all done
    return;
}

auto
mito::matrix_solvers::petsc::PETScLinearSystem::n_equations() const -> int
{
    return _n_equations;
}


// print the linear system of equations of the petsc solver
auto
mito::matrix_solvers::petsc::PETScLinearSystem::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.PETScLinearSystem");

    // print the matrix
    channel << "Matrix:" << journal::endl;
    PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));
    // print the right-hand side
    channel << "Right-hand side:" << journal::endl;
    PetscCallVoid(VecView(_rhs, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// end of file
