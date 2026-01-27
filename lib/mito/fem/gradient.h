// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once

// DESIGN NOTES
// This function computes the contravariant gradient of a shape function using the Riemannian
// gradient formula:
//   - N<D (embedded): grad(phi) = J * (J^T g J)^{-1} * dphi/dxi
//   - N=D (square): the above simplifies to grad(phi) = g^{-1} * J^{-T} * dphi/dxi

namespace mito::fem {

    template <int D, int N, class JacobianT, class MetricT, class InvMetricT, class DerivativeT>
    requires(D >= N && N > 0)
    constexpr auto compute_gradient(
        const JacobianT & J, const MetricT & g, const InvMetricT & g_inv,
        const DerivativeT & dphi_dxi)
    {
        if constexpr (N == D) {
            // square case: no embedding (triangle in 2D, segment in 1D, etc.)
            auto J_inv = tensor::inverse(J);
            auto J_invT = tensor::transpose(J_inv);
            return g_inv * (J_invT * dphi_dxi);
        } else {
            // embedded case: N < D (curves in 2D/3D, surfaces in 3D, etc.)

            auto g_induced = tensor::transpose(J) * g * J;    // induced metric
            auto g_induced_inv = tensor::inverse(g_induced);

            // for N=1, convert scalar to 1-element vector for type compatibility
            auto dphi = [&]() {
                if constexpr (N == 1)
                    return tensor::vector_t<1>{ dphi_dxi };
                else
                    return dphi_dxi;
            }();
            return J * (g_induced_inv * dphi);
        }
    }

}    // namespace mito::fem


// end of file
