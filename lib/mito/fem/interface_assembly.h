// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include "InterfaceWeakform.h"
#include "hybrid_interface_pairing.h"

namespace mito::fem {

    // Assemble interface coupling contributions into a discrete system (two-space variant).
    // Local DOF order: positive-left, positive-right, negative-left, negative-right.
    template <
        class discreteSystemT, class coordSystemT, class spacePositiveT, class spaceNegativeT,
        class bulkElementT, class interfaceElementT, int n_dof>
    auto assemble_interface(
        discreteSystemT & system, const coordSystemT & coord_system,
        const spacePositiveT & space_positive, const spaceNegativeT & space_negative,
        const InterfaceWeakform<bulkElementT, interfaceElementT, n_dof> & weakform,
        double coefficient, const std::vector<InterfacePairing<bulkElementT>> & pairings) -> void
    {
        static_assert(
            std::is_same_v<typename spacePositiveT::element_type, bulkElementT>
                && std::is_same_v<typename spaceNegativeT::element_type, bulkElementT>,
            "positive and negative spaces must match the bulk element type");

        using node_type = typename discreteSystemT::node_type;

        for (const auto & pairing : pairings) {
            assert(pairing.positive != nullptr && pairing.negative != nullptr);

            auto interface_element = make_embedded_interface_segment_element(
                coord_system, space_positive, pairing.positive_left, pairing.positive_right);

            auto elementary_matrix =
                weakform.compute_blocks(interface_element, *pairing.positive, *pairing.negative);

            const std::array<node_type, n_dof> local_nodes = {
                space_positive.node_map().at(pairing.positive_left),
                space_positive.node_map().at(pairing.positive_right),
                space_negative.node_map().at(pairing.negative_left),
                space_negative.node_map().at(pairing.negative_right),
            };

            system.template scatter_local_matrix<n_dof>(
                local_nodes, elementary_matrix, coefficient);
        }
    }

    // Assemble interface coupling contributions into a discrete system (single-space variant).
    // Used when a crack slit with duplicated mesh nodes lives inside one bulk mesh/space.
    // Local DOF order: positive-left, positive-right, negative-left, negative-right.
    template <
        class discreteSystemT, class coordSystemT, class spaceT, class bulkElementT,
        class interfaceElementT, int n_dof>
    auto assemble_interface(
        discreteSystemT & system, const coordSystemT & coord_system, const spaceT & space,
        const InterfaceWeakform<bulkElementT, interfaceElementT, n_dof> & weakform,
        double coefficient, const std::vector<InterfacePairing<bulkElementT>> & pairings) -> void
    {
        static_assert(
            std::is_same_v<typename spaceT::element_type, bulkElementT>,
            "space must match the bulk element type");

        using node_type = typename discreteSystemT::node_type;

        for (const auto & pairing : pairings) {
            assert(pairing.positive != nullptr && pairing.negative != nullptr);

            auto interface_element = make_embedded_interface_segment_element(
                coord_system, space, pairing.positive_left, pairing.positive_right);

            auto elementary_matrix =
                weakform.compute_blocks(interface_element, *pairing.positive, *pairing.negative);

            const std::array<node_type, n_dof> local_nodes = {
                space.node_map().at(pairing.positive_left),
                space.node_map().at(pairing.positive_right),
                space.node_map().at(pairing.negative_left),
                space.node_map().at(pairing.negative_right),
            };

            system.template scatter_local_matrix<n_dof>(
                local_nodes, elementary_matrix, coefficient);
        }
    }

}    // namespace mito::fem


// end of file
