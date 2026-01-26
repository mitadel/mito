// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

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

    // weakform alias
    template <class finiteElementT>
    class Weakform;

    // class discrete system
    template <function_space_c functionSpaceT, class linearSystemT>
    class DiscreteSystem;

    // class domain field
    template <fields::field_c F>
    class DomainField;

    // class finite element field
    template <class fieldValueT, class functionSpaceT>
    class FemField;

    // concept of a localizable field
    template <class F, class E>
    concept localizable_field_c = requires(const F & f, const E & e) {
        { localize(f, e) };
    };

    // concept of a fem field
    template <class F>
    concept fem_field_c = requires(F c) {
        // require that F only binds to {FemField} specializations
        []<class fieldValueT, class functionSpaceT>(const FemField<fieldValueT, functionSpaceT> &) {
        }(c);
    };
}


// end of file
