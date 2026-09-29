// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    class Environment {

      public:
        // the default constructor
        constexpr Environment() { PetscCallVoid(PetscInitializeNoArguments()); }

        // the destructor
        ~Environment() { PetscCallVoid(PetscFinalize()); }

        // the copy constructor
        Environment(const Environment &) = delete;

        // the move constructor
        Environment(Environment &&) = delete;

        // the copy assignment operator
        auto operator=(const Environment &) -> Environment & = delete;

        // the move assignment operator
        auto operator=(Environment &&) -> Environment & = delete;
    };

}    // namespace mito


// end of file
