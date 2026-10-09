// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "api.h"
#include "Matrix.h"


// constructor
mito::linalg::petsc::Matrix::Matrix(const label_type & label, index_t size) :
    _label(label),
    _size(size),
    _matrix(nullptr)
{
    // create the matrix
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &_matrix));
    PetscCallVoid(MatSetSizes(_matrix, PETSC_DECIDE, PETSC_DECIDE, size, size));

    // set the default options (do not allow the user to control the options for matrix)
    PetscCallVoid(MatSetFromOptions(_matrix));
}

// destructor
mito::linalg::petsc::Matrix::~Matrix()
{
    // destroy the matrix
    PetscCallVoid(MatDestroy(&_matrix));
}

// get the label of the matrix
auto
mito::linalg::petsc::Matrix::label() const -> label_type
{
    // easy enough
    return _label;
}

// assemble the matrix
auto
mito::linalg::petsc::Matrix::assemble() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // all done
    return;
}

// flush assemble the matrix
auto
mito::linalg::petsc::Matrix::assemble_flush() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FLUSH_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FLUSH_ASSEMBLY));

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::linalg::petsc::Matrix::insert_value(index_t row, index_t col, const scalar_t & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::add_value(index_t row, index_t col, const scalar_t & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// get the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_t row, index_t col, scalar_t & value) const -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetValue(_matrix, row, col, &value));

    // all done
    return;
}

// return the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_t row, index_t col) const -> scalar_t
{
    // the value to return
    scalar_t value;

    // delegate to PETSc
    get_value(row, col, value);

    // all done
    return value;
}

// get the number of rows/columns of the matrix
auto
mito::linalg::petsc::Matrix::size() const -> int
{
    return _size;
}


// print the matrix
auto
mito::linalg::petsc::Matrix::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.Matrix");

    // print the matrix
    channel << "Matrix:" << journal::endl;
    PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// access matrix
auto
mito::linalg::petsc::Matrix::matrix() -> matrix_type &
{
    return _matrix;
}

// end of file
