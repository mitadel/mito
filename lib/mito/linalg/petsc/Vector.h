// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    class Vector {
      private:
        // the vector type
        using vector_type = Vec;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        Vector(const label_type &, index_t);

        // destructor
        ~Vector();

      public:
        // get the label of the linear system
        auto label() const -> label_type;

        // assemble the vector
        auto assemble() -> void;

        // set the value of a vector entry
        auto insert_value(index_t, const scalar_t &) -> void;

        // add a value to a vector entry
        auto add_value(index_t, const scalar_t &) -> void;

        // accessor to the number of entries
        auto size() const -> int;

        // get the vector
        template <class vectorT>
        auto get_vector(vectorT & vector) const -> void;

        // print the vector
        auto print() const -> void;

      private:
        // the label for the linear system (this is used to prefix PETSc options)
        label_type _label;
        // the vector
        vector_type _vector;
        // the number of entries
        int _size;
    };

}    // namespace mito


// get the template definitions
#define mito_linalg_backend_petsc_Vector_icc
#include "Vector.icc"
#undef mito_linalg_backend_petsc_Vector_icc


// end of file
