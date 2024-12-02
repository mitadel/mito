// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    // computes the stress tensor for material {material}, at the state of deformation given by the
    // deformation gradient {Du}, as the numerical derivative of the strain energy density with
    // respect to the deformation gradient. The numerical derivative is computed via centered finite
    // differences with increment {eps}.
    auto numerical_stress(const auto & material, const matrix_c auto & Du, const real eps = 1.e-4)
        -> matrix_t<3>;

    // computes the tangent tensor for material {material}, at the state of deformation given by the
    // deformation gradient {Du}, as the derivative of the stress tensor with respect to the
    // deformation gradient. The numerical derivative is computed via centered finite differences
    // with increment {eps}.
    auto numerical_tangent(const auto & material, const matrix_c auto & Du, const real eps = 1.e-4)
        -> fourth_order_tensor_t<3>;

}


// get the inline definitions
#define mito_materials_numerical_icc
#include "numerical.icc"
#undef mito_materials_numerical_icc


// end of file
