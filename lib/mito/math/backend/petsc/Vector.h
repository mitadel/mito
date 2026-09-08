// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    class Vector {

      private:
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;
        // the vector type
        using vector_type = Vec;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        Vector(const label_type &);

        // destructor
        ~Vector();

      public:
        // create the vector
        auto create(index_type) -> void;

        // destroy the vector
        auto destroy() -> void;

        // get the label of the linear system
        auto label() const -> label_type;

        // assemble the vector
        auto assemble() -> void;

        // set the value of a vector entry
        auto insert_value(index_type, const scalar_type &) -> void;

        // add a value to a vector entry
        auto add_value(index_type, const scalar_type &) -> void;

        // accessor to the number of equations
        auto n_equations() const -> int;

        // get the vector
        template <class vectorT>
        auto get_vector(vectorT & vector) const -> void;

        // print the vector
        auto print() const -> void;

      private:
        // a flag to recall if this instance has initialized PETSc
        bool _initialized_petsc;
        // the label for the linear system (this is used to prefix PETSc options)
        label_type _label;
        // the vector
        vector_type _vector;
        // the number of equations
        int _n_equations;
    };

}    // namespace mito


// get the template definitions
#define mito_math_backend_petsc_Vector_icc
#include "Vector.icc"
#undef mito_math_backend_petsc_Vector_icc


// end of file
