// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // value grad block
    template <class elementT, class quadratureRuleT, fields::vector_field_c coefficientFieldT>
    class ValueGradientBlock;

    // grad grad block
    template <class elementT, class quadratureRuleT, fields::tensor_field_c coefficientFieldT>
    class GradientGradientBlock;

    // value value block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    class ValueValueBlock;

    // value rhs block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    class ValueBlock;

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
