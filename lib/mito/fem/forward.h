// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // class function space
    template <class elementT, class manifoldT, constraints::constraint_c constraintsT>
    class FunctionSpace;

    // concept of a function space
    template <class F>
    concept function_space_c = requires(F c) {
        // require that F only binds to {FunctionSpace} specializations
        []<class elementT, class manifoldT, constraints::constraint_c constraintsT>(
            const FunctionSpace<elementT, manifoldT, constraintsT> &) {
        }(c);
    };

    // class function space elements view
    template <class functionSpaceT>
    class FunctionSpaceElementsView;

    template <class lhsBlockT, class rhsBlockT>
    concept compatible_assembly_blocks_c =
        std::is_same_v<typename lhsBlockT::element_type, typename rhsBlockT::element_type>;

    // weakform alias
    template <class lhsBlockT, class rhsBlockT>
    requires compatible_assembly_blocks_c<lhsBlockT, rhsBlockT>
    class Weakform;

    // class discrete system
    template <function_space_c functionSpaceT, class weakformT, class linearSystemT>
    class DiscreteSystem;

    // class domain field
    template <fields::field_c F>
    class DomainField;

    // class finite element field
    template <class fieldValueT>
    class FemField;

    // concept of a fem field
    template <class F>
    concept fem_field_c = requires(F c) {
        // require that F only binds to {FemField} specializations
        []<class fieldValueT>(const FemField<fieldValueT> &) {
        }(c);
    };
}


// end of file
