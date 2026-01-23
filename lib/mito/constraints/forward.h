// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::constraints {

    // class Dirichlet boundary condition
    template <class meshT, fields::field_c fieldT>
    class Dirichlet;

    // concept of a constraint
    template <class F>
    concept constraint_c = requires(F c) {
        // require that F only binds to {Dirichlet} specializations
        []<class meshT, fields::field_c fieldT>(const Dirichlet<meshT, fieldT> &) {
        }(c);
    };
}


// end of file
