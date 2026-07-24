// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include "InterfaceCouplingBlock.h"
#include "InterfaceJumpJumpMassBlock.h"

#include <cmath>

namespace mito::fem::blocks {

    template <class bulkElementT, class interfaceElementT>
    class InterfaceAverageAverageGradBlock :
        public InterfaceCouplingBlock<
            bulkElementT, interfaceElementT, tensor::matrix_t<2 * interfaceElementT::n_nodes>> {

      public:
        using bulk_element_type = bulkElementT;
        using interface_element_type = interfaceElementT;
        static constexpr int n_edge_nodes = interface_element_type::n_nodes;
        static constexpr int n_dof = 2 * n_edge_nodes;
        using elementary_block_type = tensor::matrix_t<n_dof>;

      public:
        // prefactor is (w * kappa_m / 4)
        constexpr explicit InterfaceAverageAverageGradBlock(double w_kappa_m_over_4) :
            _w_kappa_m_over_4(w_kappa_m_over_4)
        {}

        auto compute(
            const interface_element_type & interface_element,
            const bulk_element_type & /* bulk_positive */,
            const bulk_element_type & /* bulk_negative */) const -> elementary_block_type override
        {
            const auto h = detail::interface_segment_physical_length(interface_element);
            const auto G00 = 1.0 / h;
            const auto G01 = -1.0 / h;
            const double G[2][2] = { { G00, G01 }, { G01, G00 } };

            elementary_block_type K{};
            for (int r = 0; r < n_dof; ++r) {
                for (int c = 0; c < n_dof; ++c) {
                    const int lr = r % 2;
                    const int lc = c % 2;
                    // same sign pattern as the Chapter 5 verification driver (diffusion term)
                    K[{ r, c }] = -_w_kappa_m_over_4 * G[lr][lc];
                }
            }
            return K;
        }

      private:
        double _w_kappa_m_over_4;
    };

}    // namespace mito::fem::blocks
