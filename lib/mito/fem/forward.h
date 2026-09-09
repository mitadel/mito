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
    concept all_same_elementary_shape_c =
        // require the same underlying element type
        (std::same_as<typename firstBlockT::elementary_shape, typename blockTs::elementary_shape>
         && ...);

    template <class... Types>
    concept same_elementary_shape_blocks_c = []<class First, class... Rest>() {
        return all_same_elementary_shape_c<First, Rest...>;
    }.template operator()<Types...>();

    // concept of blocks with the same finite element type
    template <class firstBlockT, class... blockTs>
    concept all_same_finite_element_blocks_c =
        // require the same underlying element type
        (std::same_as<typename firstBlockT::element_type, typename blockTs::element_type> && ...);

    template <class... Types>
    concept same_finite_element_blocks_c = []<class First, class... Rest>() {
        return all_same_finite_element_blocks_c<First, Rest...>;
    }.template operator()<Types...>();

    // class stiffness mixin
    template <class blockT>
    class StiffnessMixin;

    // class load mixin
    template <class blockT>
    class LoadMixin;

    // transient weakform alias
    template <class... mixinTs>
    requires same_finite_element_blocks_c<mixinTs...>
    class TransientWeakform;

    // weakform alias
    template <class lhsBlockT, class rhsBlockT>
    requires same_finite_element_blocks_c<lhsBlockT, rhsBlockT>
    class Weakform;

    // class discrete system
    template <function_space_c functionSpaceT, class weakformT, class linearSystemT>
    class DiscreteSystem;

    // class discrete transient system
    template <function_space_c functionSpaceT, class weakformT, class linearSystemT>
    class DiscreteTransientSystem;

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
