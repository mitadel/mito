// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include "forward.h"
#include "externals.h"
#include "PETScLinearSystem.h"
#include "KrylovSolver.h"


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

// constructor
mito::matrix_solvers::petsc::KrylovSolver::KrylovSolver(
    typename math_backend_type::matrix_type & matrix, const options_type & options) :
    _matrix(matrix),
    _options_prefix("petsc_ksp_")
{
    // create the Krylov solver
    PetscCallVoid(KSPCreate(PETSC_COMM_WORLD, &_ksp));
    PetscCallVoid(KSPSetOperators(_ksp, _matrix.matrix(), _matrix.matrix()));
    PetscCallVoid(KSPSetOptionsPrefix(_ksp, _options_prefix.c_str()));

    // prepend the prefix {_options_prefix} to each of the space-separated options in input
    auto prefixed_options = prepend_options_prefix(options, _options_prefix);

    // record the options with PETSc
    PetscCallVoid(PetscOptionsInsertString(PETSC_NULLPTR, prefixed_options.c_str()));

    // configure the Krylov solver with the options
    PetscCallVoid(KSPSetFromOptions(_ksp));
}

// destructor
mito::matrix_solvers::petsc::KrylovSolver::~KrylovSolver()
{
    // destroy the Krylov solver
    PetscCallVoid(KSPDestroy(&_ksp));
}

// set petsc options
auto
mito::matrix_solvers::petsc::KrylovSolver::set_options(const options_type & options) -> void
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

// // solve the linear system
// auto
// mito::matrix_solvers::petsc::KrylovSolver::solve() -> void
// {
//     // solve the linear system
//     PetscCallVoid(KSPSolve(_ksp, _linear_system._rhs, _linear_system._solution));

//     // all done
//     return;
// }

// // print the linear system of equations of the petsc solver
// auto
// mito::matrix_solvers::petsc::KrylovSolver::print() const -> void
// {
//     // create a reporting channel
//     journal::info_t channel("mito.solvers.petsc.KrylovSolver");

//     // print the linear system
//     _linear_system.print();

//     // all done
//     return;
// }

// end of file
