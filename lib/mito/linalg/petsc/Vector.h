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

      public:
        // constructor
        Vector(index_t);

        // copy constructor
        Vector(const Vector &) = delete;

        // move constructor
        Vector(Vector &&) = delete;

        // copy assignment operator
        Vector & operator=(const Vector &) = delete;

        // move assignment operator
        Vector & operator=(Vector &&) = delete;

        // destructor
        ~Vector();

      public:
        // assemble the vector
        auto assemble() -> void;

        // set the value of a vector entry
        auto insert_value(index_t, const scalar_t &) -> void;

        // add a value to a vector entry
        auto add_value(index_t, const scalar_t &) -> void;

        // get the size of the vector
        auto size(index_t &) const -> void;

        // return the size of the vector
        auto size() const -> index_t;

        // get the vector
        template <class vectorT>
        auto get_vector(vectorT & vector) const -> void;

        // print the vector
        auto print() const -> void;

        // get petsc vector
        auto get() const -> vector_type const &;

      private:
        // the vector
        vector_type _vector;
    };

}    // namespace mito


// get the template definitions
#define mito_linalg_backend_petsc_Vector_icc
#include "Vector.icc"
#undef mito_linalg_backend_petsc_Vector_icc


// end of file
