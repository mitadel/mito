// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include "InterfaceCouplingBlock.h"

#include <cmath>

namespace mito::fem::blocks {

    namespace detail {
        // physical length of an embedded P1 segment
        template <class interfaceElementT>
        inline auto interface_segment_physical_length(const interfaceElementT & interface_element)
            -> tensor::scalar_t
        {
            using ref_t = typename interfaceElementT::canonical_element_type;
            typename ref_t::parametric_coordinates_type xi0{ 0.0 };
            typename ref_t::parametric_coordinates_type xi1{ 1.0 };
            auto p0 = interface_element.parametrization()(xi0);
            auto p1 = interface_element.parametrization()(xi1);
            auto d = p1 - p0;
            tensor::scalar_t s = 0.0;
            constexpr int Ddim = interfaceElementT::D;
            for (int i = 0; i < Ddim; ++i) {
                s += d[i] * d[i];
            }
            return std::sqrt(s);
        }
    }    // namespace detail

    template <class bulkElementT, class interfaceElementT>
    class InterfaceJumpJumpMassBlock :
        public InterfaceCouplingBlock<
            bulkElementT, interfaceElementT, tensor::matrix_t<2 * interfaceElementT::n_nodes>> {

      public:
        using bulk_element_type = bulkElementT;
        using interface_element_type = interfaceElementT;
        static constexpr int n_edge_nodes = interface_element_type::n_nodes;
        static constexpr int n_dof = 2 * n_edge_nodes;
        using elementary_block_type = tensor::matrix_t<n_dof>;

      public:
        constexpr explicit InterfaceJumpJumpMassBlock(double kappa_m_over_w) :
            _kappa_m_over_w(kappa_m_over_w)
        {}

        auto compute(
            const interface_element_type & interface_element,
            const bulk_element_type & /* bulk_positive */,
            const bulk_element_type & /* bulk_negative */) const -> elementary_block_type override
        {
            const auto h = detail::interface_segment_physical_length(interface_element);
            const auto M00 = h * 2.0 / 6.0;
            const auto M01 = h * 1.0 / 6.0;
            const double M[2][2] = { { M00, M01 }, { M01, M00 } };

            const double jump_sign[4][4] = {
                { +1, +1, -1, -1 },
                { +1, +1, -1, -1 },
                { -1, -1, +1, +1 },
                { -1, -1, +1, +1 },
            };

            elementary_block_type K{};
            for (int r = 0; r < n_dof; ++r) {
                for (int c = 0; c < n_dof; ++c) {
                    const int lr = r % 2;
                    const int lc = c % 2;
                    K[{ r, c }] = _kappa_m_over_w * jump_sign[r][c] * M[lr][lc];
                }
            }
            return K;
        }

      private:
        double _kappa_m_over_w;
    };

}    // namespace mito::fem::blocks
