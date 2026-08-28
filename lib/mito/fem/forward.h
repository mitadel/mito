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

    // concept of blocks with the same finite element type
    template <class firstBlockT, class... blockTs>
    concept same_finite_element_blocks_c =
        // require the same underlying element type
        (std::same_as<typename firstBlockT::element_type, typename blockTs::element_type> && ...);

    // concept of blocks with the same elementary shape
    template <class firstBlockT, class... blockTs>
    concept same_elementary_shape_blocks_c =
        // require the same elementary shape
        (std::same_as<typename firstBlockT::elementary_shape, typename blockTs::elementary_shape>
         && ...);

    // weakform alias
    template <class lhsBlockT, class rhsBlockT>
    requires same_finite_element_blocks_c<lhsBlockT, rhsBlockT>
    class Weakform;

    // a contribution to a discrete system: a weakform to be assembled on a function space
    template <function_space_c functionSpaceT, class weakformT>
    struct Contribution;

    // concept of a contribution
    template <class C>
    concept contribution_c = requires(C c) {
        // require that C only binds to {Contribution} specializations
        []<function_space_c functionSpaceT, class weakformT>(
            const Contribution<functionSpaceT, weakformT> &) {
        }(c);
    };

    // class discrete system
    template <class linearSystemT, contribution_c... contributionTs>
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
