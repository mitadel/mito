// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "Matrix.h"


// constructor
mito::math_backend::petsc::Matrix::Matrix(const label_type & label) : _label(label), _n_equations(0)
{}

// destructor
mito::math_backend::petsc::Matrix::~Matrix() {}

// allocate memory for the matrix
auto
mito::math_backend::petsc::Matrix::create(index_type size) -> void
{
    // take note of the number of equations
    _n_equations = size;

    // create the matrix
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &_matrix));
    PetscCallVoid(MatSetSizes(_matrix, PETSC_DECIDE, PETSC_DECIDE, size, size));

    // set the default options (do not allow the user to control the options for matrix)
    PetscCallVoid(MatSetFromOptions(_matrix));

    // all done
    return;
}

// free memory for the matrix
auto
mito::math_backend::petsc::Matrix::destroy() -> void
{
    // destroy the matrix
    PetscCallVoid(MatDestroy(&_matrix));

    // all done
    return;
}

// get the label of the matrix
auto
mito::math_backend::petsc::Matrix::label() const -> label_type
{
    // easy enough
    return _label;
}

// assemble the matrix
auto
mito::math_backend::petsc::Matrix::assemble() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // // show the matrix
    // PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// flush assemble the matrix
auto
mito::math_backend::petsc::Matrix::assembleFlush() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FLUSH_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FLUSH_ASSEMBLY));

    // // show the matrix
    // PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::math_backend::petsc::Matrix::insert_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::math_backend::petsc::Matrix::add_value(
    index_type row, index_type col, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// get the value of the matrix entry at ({row}, {col})
auto
mito::math_backend::petsc::Matrix::get_value(
    index_type row, index_type col, scalar_type & value) const -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetValue(_matrix, row, col, &value));

    // all done
    return;
}

// return the value of the matrix entry at ({row}, {col})
auto
mito::math_backend::petsc::Matrix::get_value(index_type row, index_type col) const -> scalar_type
{
    // the value to return
    scalar_type value;

    // delegate to PETSc
    get_value(row, col, value);

    // all done
    return value;
}

auto
mito::math_backend::petsc::Matrix::n_equations() const -> int
{
    return _n_equations;
}


// print the matrix
auto
mito::math_backend::petsc::Matrix::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.Matrix");

    // print the matrix
    channel << "Matrix:" << journal::endl;
    PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// end of file
