// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "api.h"
#include "LinearSystem.h"


// constructor
mito::linalg::petsc::LinearSystem::LinearSystem(const label_type & label, index_t size) :
    _label(label),
    _n_equations(size)
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
}

// destructor
mito::linalg::petsc::LinearSystem::~LinearSystem()
{
    // destroy the matrix, right-hand side, solution
    PetscCallVoid(MatDestroy(&_matrix));
    PetscCallVoid(VecDestroy(&_solution));
    PetscCallVoid(VecDestroy(&_rhs));
}

// get the label of the linear system
auto
mito::linalg::petsc::LinearSystem::label() const -> label_type
{
    // easy enough
    return _label;
}

// assemble the linear system
auto
mito::linalg::petsc::LinearSystem::assemble() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::linalg::petsc::LinearSystem::insert_matrix_value(
    index_t row, index_t col, const scalar_t & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::linalg::petsc::LinearSystem::add_matrix_value(
    index_t row, index_t col, const scalar_t & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// set the right-hand side entry at {row} to {value}
auto
mito::linalg::petsc::LinearSystem::insert_rhs_value(index_t row, const scalar_t & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to right-hand side entry at {row}
auto
mito::linalg::petsc::LinearSystem::add_rhs_value(index_t row, const scalar_t & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_rhs, row, value, ADD_VALUES));

    // all done
    return;
}

auto
mito::linalg::petsc::LinearSystem::n_equations() const -> int
{
    return _n_equations;
}


// print the linear system of equations of the petsc solver
auto
mito::linalg::petsc::LinearSystem::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.LinearSystem");

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
