// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include "elements/elements_library.h"

#include "mito/functions/public.h"
#include "mito/geometry/public.h"
#include "mito/manifolds/public.h"
#include "mito/mesh/public.h"

#include <cmath>
#include <vector>

namespace mito::fem {

    // One bulk element on each side of an interface edge, keyed by four interface mesh nodes.
    // "positive" = side in the direction of the interface normal; "negative" = opposite side.
    // In the two-space case positive_left == negative_left (shared geometric node, different
    // discretization nodes via separate node_maps). In the single-space case all four are
    // distinct mesh vertices at the crack slit.
    template <class bulkElementT>
    struct InterfacePairing {
        using node_type = typename bulkElementT::cell_type::node_type;
        node_type positive_left;
        node_type positive_right;
        node_type negative_left;
        node_type negative_right;
        const bulkElementT * positive;
        const bulkElementT * negative;
    };

    // Return true if the triangle cell contains both mesh nodes (as an edge).
    template <class triangleCellT, class nodeT>
    inline auto triangle_cell_contains_edge(
        const triangleCellT & cell, const nodeT & na, const nodeT & nb) -> bool
    {
        int count = 0;
        for (const auto & n : cell.nodes()) {
            if (n == na || n == nb) {
                ++count;
            }
        }
        return count == 2;
    }

    // find the first bulk finite element whose geometric cell contains the edge (na, nb).
    template <class functionSpaceT, class nodeT>
    inline auto find_bulk_element_pointer_with_edge(
        const functionSpaceT & space, const nodeT & na, const nodeT & nb) -> const
        typename functionSpaceT::element_type *
    {
        for (const auto & element : space.elements()) {
            if (triangle_cell_contains_edge(element.cell(), na, nb)) {
                return &element;
            }
        }
        return nullptr;
    }

    // embedded P1 segment element on the interface, using the positive-side trace for connectivity
    template <class coordSystemT, class spaceT>
    inline auto make_embedded_interface_segment_element(
        const coordSystemT & coord_system, const spaceT & space,
        const typename spaceT::mesh_node_type & mesh_node_left,
        const typename spaceT::mesh_node_type & mesh_node_right)
    {
        using coordinates_t = typename coordSystemT::coordinates_type;
        constexpr int D = coordinates_t::dim;
        using segment_t = geometry::segment_t<D>;

        auto geometric_simplex = geometry::segment<D>({ mesh_node_left, mesh_node_right });
        auto mesh = mesh::mesh<segment_t>();
        mesh.insert({ mesh_node_left, mesh_node_right });

        const auto c0 = coord_system.coordinates(mesh_node_left->point());
        const auto c1 = coord_system.coordinates(mesh_node_right->point());
        tensor::scalar_t dx = c1[0] - c0[0];
        tensor::scalar_t dy = c1[1] - c0[1];
        const auto len = std::sqrt(dx * dx + dy * dy);
        tensor::scalar_t nx = 0.0;
        tensor::scalar_t ny = 1.0;
        if (len > 0.0) {
            nx = -dy / len;
            ny = dx / len;
        }
        tensor::vector_t<D> normal_vec{};
        normal_vec[0] = nx;
        if constexpr (D >= 2) {
            normal_vec[1] = ny;
        }
        auto normal_field = functions::constant<coordinates_t>(normal_vec);

        auto manifold = manifolds::submanifold(mesh, coord_system, normal_field);
        using interface_element_t = isoparametric_simplex_t<1, decltype(manifold)>;
        return interface_element_t(
            geometric_simplex, coord_system,
            typename interface_element_t::connectivity_type{ space.node_map().at(mesh_node_left),
                                                             space.node_map().at(mesh_node_right) },
            manifold.volume_form());
    }

    // build pairings for a single row of mesh nodes shared by two separate bulk spaces
    template <class functionSpaceT>
    inline auto build_interface_pairings(
        const functionSpaceT & positive_space, const functionSpaceT & negative_space,
        const std::vector<typename functionSpaceT::mesh_node_type> & interface_row_nodes,
        int n_segments) -> std::vector<InterfacePairing<typename functionSpaceT::element_type>>
    {
        using bulk_element_t = typename functionSpaceT::element_type;
        using pairing_t = InterfacePairing<bulk_element_t>;

        std::vector<pairing_t> pairings;
        pairings.reserve(static_cast<std::size_t>(n_segments));

        for (int seg = 0; seg < n_segments; ++seg) {
            const auto & nL = interface_row_nodes[static_cast<std::size_t>(seg)];
            const auto & nR = interface_row_nodes[static_cast<std::size_t>(seg + 1)];
            const auto * pos_el = find_bulk_element_pointer_with_edge(positive_space, nL, nR);
            const auto * neg_el = find_bulk_element_pointer_with_edge(negative_space, nL, nR);
            pairings.push_back(pairing_t{ nL, nR, nL, nR, pos_el, neg_el });
        }
        return pairings;
    }

    // build pairings for a crack slit inside a single bulk mesh/space
    // The caller provides two parallel vectors of mesh nodes along the crack: one for the
    // positive side, one for the negative side (paired by index, same geometric coordinates,
    // distinct node identities)
    template <class functionSpaceT>
    inline auto build_interface_pairings(
        const functionSpaceT & space,
        const std::vector<typename functionSpaceT::mesh_node_type> & positive_nodes,
        const std::vector<typename functionSpaceT::mesh_node_type> & negative_nodes, int n_segments)
        -> std::vector<InterfacePairing<typename functionSpaceT::element_type>>
    {
        using bulk_element_t = typename functionSpaceT::element_type;
        using pairing_t = InterfacePairing<bulk_element_t>;

        assert(positive_nodes.size() == static_cast<std::size_t>(n_segments + 1));
        assert(negative_nodes.size() == static_cast<std::size_t>(n_segments + 1));

        std::vector<pairing_t> pairings;
        pairings.reserve(static_cast<std::size_t>(n_segments));

        for (int seg = 0; seg < n_segments; ++seg) {
            const auto & pL = positive_nodes[static_cast<std::size_t>(seg)];
            const auto & pR = positive_nodes[static_cast<std::size_t>(seg + 1)];
            const auto & nL = negative_nodes[static_cast<std::size_t>(seg)];
            const auto & nR = negative_nodes[static_cast<std::size_t>(seg + 1)];
            const auto * pos_el = find_bulk_element_pointer_with_edge(space, pL, pR);
            const auto * neg_el = find_bulk_element_pointer_with_edge(space, nL, nR);
            pairings.push_back(pairing_t{ pL, pR, nL, nR, pos_el, neg_el });
        }
        return pairings;
    }

}    // namespace mito::fem
