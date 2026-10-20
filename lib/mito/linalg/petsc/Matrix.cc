// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// support
#include "public.h"


// constructor
mito::linalg::petsc::Matrix::Matrix(index_type size) : _matrix(nullptr)
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

// copy constructor
mito::linalg::petsc::Matrix::Matrix(const Matrix & other) : _matrix(nullptr)
{
    // duplicate matrix
    PetscCallVoid(MatDuplicate(other._matrix, MAT_COPY_VALUES, &_matrix));
}

// move constructor
mito::linalg::petsc::Matrix::Matrix(Matrix && other) noexcept : _matrix(other._matrix)
{
    // remove matrix
    other._matrix = nullptr;
}

// copy assignment operator
auto
mito::linalg::petsc::Matrix::operator=(const Matrix & other) -> Matrix &
{
    // prevent self-copy
    if (this == &other) {
        return *this;
    }

    // copy and swap
    Matrix tmp(other);
    std::swap(_matrix, tmp._matrix);

    // returning deletes {tmp}
    return *this;
}

// move assignment operator
auto
mito::linalg::petsc::Matrix::operator=(Matrix && other) noexcept -> Matrix &
{
    // prevent self-move
    if (this != &other) {
        // free the matrix
        MatDestroy(&_matrix);
        // swap the matrix
        _matrix = other._matrix;
        other._matrix = nullptr;
    }

    return *this;
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
mito::linalg::petsc::Matrix::insert_value(index_type row, index_type col, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::add_value(index_type row, index_type col, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// get the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_type row, index_type col, scalar_type & value) const
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetValue(_matrix, row, col, &value));

    // all done
    return;
}

// return the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_type row, index_type col) const -> scalar_type
{
    // the value to return
    scalar_type value;

    // delegate to PETSc
    get_value(row, col, value);

    // all done
    return value;
}

// add scaled matrix using MatAXPY
auto
mito::linalg::petsc::Matrix::add(
    const scalar_type & alpha, const Matrix & X, sparsity_pattern pattern) -> void
{
    // delegate to PETsc
    PetscCallVoid(MatAXPY(_matrix, alpha, X._matrix, static_cast<MatStructure>(pattern)));
}

// get the number of rows/columns of the matrix
auto
mito::linalg::petsc::Matrix::size(index_type & matrix_size) const -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetSize(_matrix, &matrix_size, nullptr));

    // all done
    return;
}

// return the number of rows/columns of the matrix
auto
mito::linalg::petsc::Matrix::size() const -> index_type
{
    // the value to return
    index_type matrix_size;

    // delegate to PETSc
    size(matrix_size);

    // all done
    return matrix_size;
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

// get petsc matrix
auto
mito::linalg::petsc::Matrix::get() const -> matrix_type const &
{
    return _matrix;
}

// end of file
