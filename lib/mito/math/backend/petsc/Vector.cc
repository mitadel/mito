// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "Vector.h"


// constructor
mito::math_backend::petsc::Vector::Vector(const label_type & label) : _label(label), _n_equations(0)
{}

// destructor
mito::math_backend::petsc::Vector::~Vector() {}

// allocate memory for the vector
auto
mito::math_backend::petsc::Vector::create(index_type size) -> void
{
    // take note of the number of equations
    _n_equations = size;

    // create the vector
    PetscCallVoid(VecCreate(PETSC_COMM_WORLD, &_vector));
    PetscCallVoid(VecSetSizes(_vector, PETSC_DECIDE, size));

    // set the default options (do not allow the user to control the options for vector)
    PetscCallVoid(VecSetFromOptions(_vector));

    // all done
    return;
}

// free memory for the vector
auto
mito::math_backend::petsc::Vector::destroy() -> void
{
    // destroy the vector
    PetscCallVoid(VecDestroy(&_vector));

    // all done
    return;
}

// get the label of the vector
auto
mito::math_backend::petsc::Vector::label() const -> label_type
{
    // easy enough
    return _label;
}

// set the vector entry at {row} to {value}
auto
mito::math_backend::petsc::Vector::insert_value(index_type row, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_vector, row, value, INSERT_VALUES));

    // all done
    return;
}

// assemble the vector
auto
mito::math_backend::petsc::Vector::assemble() -> void
{
    // assemble vector
    PetscCallVoid(VecAssemblyBegin(_vector));
    PetscCallVoid(VecAssemblyEnd(_vector));

    // // show the vector
    // PetscCallVoid(VecView(_vector, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// add {value} to vector entry at {row}
auto
mito::math_backend::petsc::Vector::add_value(index_type row, const scalar_type & value) -> void
{
    // delegate to PETSc
    PetscCallVoid(VecSetValue(_vector, row, value, ADD_VALUES));

    // all done
    return;
}

// get the number of entries in the vector
auto
mito::math_backend::petsc::Vector::n_equations() const -> int
{
    return _n_equations;
}


// print the linear system of equations of the petsc solver
auto
mito::math_backend::petsc::Vector::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.Vector");

    // print the vector
    channel << "Vector:" << journal::endl;
    PetscCallVoid(VecView(_vector, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// end of file
