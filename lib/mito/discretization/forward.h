// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // class discretization node
    class DiscretizationNode;

    // class discrete field
    template <class keyT, class Y>
    class DiscreteField;

    // class function space
    template <class elementT, constraints::constraint_c constraintsT>
    class FunctionSpace;

    // concept of a function space
    template <class F>
    concept function_space_c = requires(F c) {
        // require that F only binds to {FunctionSpace} specializations
        []<class elementT, constraints::constraint_c constraintsT>(
            const FunctionSpace<elementT, constraintsT> &) {
        }(c);
    };

    // class discrete system
    template <function_space_c functionSpaceT, class linearSystemT>
    class DiscreteSystem;
}


// end of file
