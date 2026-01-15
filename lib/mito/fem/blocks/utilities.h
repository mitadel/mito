// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // Helper function to compute the Jacobian measure (integration scaling factor)
    // for different Jacobian matrix shapes.

    // For square Jacobians (NxN): return |det(J)|
    template <class matrixT>
    requires(tensor::square_matrix_c<matrixT>)
    constexpr auto jacobian_measure(const matrixT & J)
    {
        return std::abs(tensor::determinant(J));
    }

    // For 2x1 Jacobians (embedded 1D in 2D): return sqrt(det(J^T * J))
    // This is the square root of the Gram determinant as used in the area formula
    template <class matrixT>
    requires(tensor::matrix_c<matrixT> && matrixT::dims[0] == 2 && matrixT::dims[1] == 1)
    constexpr auto jacobian_measure(const matrixT & J)
    {
        auto JtJ = tensor::transpose(J) * J;  // 1x1 matrix
        return std::sqrt(JtJ[{ 0, 0 }]);
    }

}    // namespace mito


// end of file
