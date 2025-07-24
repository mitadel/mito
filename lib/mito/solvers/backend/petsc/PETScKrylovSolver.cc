// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "PETScLinearSystem.h"
#include "PETScKrylovSolver.h"


// constructor
mito::solvers::petsc::PETScKrylovSolver::PETScKrylovSolver(linear_system_type & linear_system) :
    _linear_system(linear_system),
    _options_prefix(linear_system.label() + "_")
{}

// destructor
mito::solvers::petsc::PETScKrylovSolver::~PETScKrylovSolver() {}

// create the Krylov solver
auto
mito::solvers::petsc::PETScKrylovSolver::create() -> void
{
    // create the Krylov solver
    PetscCallVoid(KSPCreate(PETSC_COMM_WORLD, &_ksp));
    PetscCallVoid(KSPSetOperators(_ksp, _linear_system._matrix, _linear_system._matrix));
    PetscCallVoid(KSPSetOptionsPrefix(_ksp, _options_prefix.c_str()));

    // all done
    return;
}

// destroy the Krylov solver
auto
mito::solvers::petsc::PETScKrylovSolver::destroy() -> void
{
    // destroy the Krylov solver
    PetscCallVoid(KSPDestroy(&_ksp));

    // all done
    return;
}

namespace {
    // helper function to prepend the prefix {prefix} to each of the space-separated
    // options leading with '-'
    auto prepend_options_prefix(const std::string & options, const std::string & prefix)
        -> std::string
    {
        // split the {options} string into words and store them in a vector
        size_t pos_old = 0;
        size_t pos_new = 0;
        std::vector<std::string> options_vector;
        while ((pos_new = options.find(" ", pos_old)) != std::string::npos) {
            std::string word_string = options.substr(pos_old, pos_new - pos_old);
            options_vector.push_back(word_string);
            pos_old = pos_new + 1;
        }
        std::string word_string = options.substr(pos_old, pos_new - pos_old);
        options_vector.push_back(word_string);

        // for each word, if the leading character is '-', insert the {prefix} between '-' and the
        // rest of the word
        for (auto & word : options_vector) {
            if (word[0] == '-') {
                word.erase(0, 1);
                word = "-" + prefix + word;
            }
        }

        // concatenate the words into a single string
        std::string prefixed_options;
        for (auto & word : options_vector) {
            prefixed_options += word + " ";
        }

        // all done
        return prefixed_options;
    }
}

// set petsc options
auto
mito::solvers::petsc::PETScKrylovSolver::set_options(const options_type & options) -> void
{
    // prepend the prefix {_options_prefix} to each of the space-separated options in input
    auto prefixed_options = prepend_options_prefix(options, _options_prefix);

    // record the options with PETSc
    PetscCallVoid(PetscOptionsInsertString(PETSC_NULLPTR, prefixed_options.c_str()));

    // configure the Krylov solver with the options
    PetscCallVoid(KSPSetFromOptions(_ksp));

    // // show all options that have been set
    // PetscOptionsView(PETSC_NULLPTR, PETSC_VIEWER_STDOUT_WORLD);

    // all done
    return;
}

// solve the linear system
auto
mito::solvers::petsc::PETScKrylovSolver::solve() -> void
{
    // assemble the linear system
    _linear_system.assemble();

    // solve the linear system
    PetscCallVoid(KSPSolve(_ksp, _linear_system._rhs, _linear_system._solution));

    // all done
    return;
}

// print the linear system of equations of the petsc solver
auto
mito::solvers::petsc::PETScKrylovSolver::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.PETScKrylovSolver");

    // print the linear system
    _linear_system.print();

    // all done
    return;
}

// end of file
