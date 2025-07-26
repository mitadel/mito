// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // assembly block
    template <class elementT, class blockT>
    class AssemblyBlock;

    // grad grad block
    template <class elementT, class quadratureRuleT>
    class GradGradBlock;

    // source term block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    class SourceTermBlock;

    // L2 norm block
    template <class elementT, class quadratureRuleT, fields::field_c fieldT>
    class L2NormBlock;

}


// end of file
