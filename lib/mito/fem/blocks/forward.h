// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // grad grad block
    template <class elementT, class quadratureRuleT>
    class GradGradBlock;

    // mass block
    template <class elementT, class quadratureRuleT>
    class MassBlock;

    // source term block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    class SourceTermBlock;

    // L2 norm block for a function defined at quadrature points in parametric coordinates
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    // require that {functionT} is a function in parametric coordinates
    requires(std::is_same_v<
             typename functionT::input_type, typename quadratureRuleT::quadrature_point_type>)
    class L2NormBlock;

    // concept of {T} being a finite element of type {elementT}
    template <class T, class finiteElementT>
    concept element_of_type_c = std::same_as<typename T::traits, finiteElementT>;
}


// end of file
