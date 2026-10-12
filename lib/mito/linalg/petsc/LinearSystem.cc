// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// support
#include "public.h"


// constructor
mito::linalg::petsc::LinearSystem::LinearSystem(index_t size) :
    _matrix(size),
    _rhs(size),
    _solution(size)
{
    ;
}

// assemble the linear system
auto
mito::linalg::petsc::LinearSystem::assemble() -> void
{
    // assemble matrix
    _matrix.assemble();

    // all done
    return;
}

// assemble the linear system
auto
mito::linalg::petsc::LinearSystem::assemble_flush() -> void
{
    // assemble matrix
    _matrix.assemble_flush();

    // all done
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::linalg::petsc::LinearSystem::insert_matrix_value(
    index_t row, index_t col, const scalar_t & value) -> void
{
    // insert the value into the matrix
    _matrix.insert_value(row, col, value);

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::linalg::petsc::LinearSystem::add_matrix_value(
    index_t row, index_t col, const scalar_t & value) -> void
{
    // add the value to the matrix
    _matrix.add_value(row, col, value);

    // all done
    return;
}

// set the right-hand side entry at {row} to {value}
auto
mito::linalg::petsc::LinearSystem::insert_rhs_value(index_t row, const scalar_t & value) -> void
{
    // insert the value into the right-hand side
    _rhs.insert_value(row, value);

    // all done
    return;
}

// add {value} to right-hand side entry at {row}
auto
mito::linalg::petsc::LinearSystem::add_rhs_value(index_t row, const scalar_t & value) -> void
{
    // add the value to the right-hand side
    _rhs.add_value(row, value);

    // all done
    return;
}

// return the number of equations in the linear system
auto
mito::linalg::petsc::LinearSystem::n_equations() const -> index_t
{
    // all done
    return _matrix.size();
}


// print the linear system of equations of the petsc solver
auto
mito::linalg::petsc::LinearSystem::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.LinearSystem");

    // print the matrix
    _matrix.print();

    // print the right-hand side
    _rhs.print();

    // all done
    return;
}

// get the matrix
auto
mito::linalg::petsc::LinearSystem::get_matrix() const -> matrix_type const &
{
    return _matrix;
}

// get the right-hand side vector
auto
mito::linalg::petsc::LinearSystem::get_rhs() const -> vector_type const &
{
    return _rhs;
}

// get the solution vector
auto
mito::linalg::petsc::LinearSystem::get_solution() const -> vector_type const &
{
    return _solution;
}

// end of file
