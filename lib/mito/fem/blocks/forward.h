// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // assembly block
    template <class elementT, class blockT>
    class AssemblyBlock;

    // grad grad block
    template <class elementT, class quadratureRuleT>
    class GradGradBlock;

    // source term block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    class SourceTermBlock;

    // L2 norm block for a function defined at quadrature points in barycentric coordinates
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    // require that {functionT} is a function in barycentric coordinates
    requires(std::is_same_v<
             typename functionT::input_type, typename quadratureRuleT::quadrature_point_type>)
    class L2NormBlock;
}


// end of file
