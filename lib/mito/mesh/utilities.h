// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


#include "tetra.h"


namespace mito::mesh {

    // populate a container with a collection of all nodes in a mesh
    template <class nodesCollectionT, class cellT>
    inline auto get_nodes(const mesh_t<cellT> & mesh, nodesCollectionT & nodes) -> void
    {
        for (const auto & cell : mesh.cells()) {
            for (const auto & node : cell.nodes()) {
                nodes.insert(node);
            }
        }

        // all done
        return;
    }

    // split a 2D mesh along a horizontal line y = y_interface
    template <class cellT, class coordSystemT>
    inline auto split_mesh_at_horizontal_interface(
        const mesh_t<cellT> & mesh, const coordSystemT & coord_system, double y_interface,
        double tolerance = 1.0e-12)
        -> std::tuple<mesh_t<cellT>, mesh_t<cellT>, std::vector<typename cellT::node_type>>
    {
        static_assert(cellT::dim == 2, "split_mesh_at_horizontal_interface requires 2D cells");

        using node_type = typename cellT::node_type;

        auto top_mesh = mesh_t<cellT>();
        auto bottom_mesh = mesh_t<cellT>();
        std::vector<node_type> interface_nodes;

        // first pass: identify interface nodes
        std::unordered_set<node_type, utilities::hash_function<node_type>> interface_node_set;
        std::unordered_set<node_type, utilities::hash_function<node_type>> seen_nodes;

        for (const auto & cell : mesh.cells()) {
            for (const auto & node : cell.nodes()) {
                if (seen_nodes.insert(node).second) {
                    // get y-coordinate (index 1 for 2D Cartesian)
                    auto coords = coord_system.coordinates(node->point());
                    double y_coord = coords[1];

                    if (std::abs(y_coord - y_interface) < tolerance) {
                        interface_node_set.insert(node);
                    }
                }
            }
        }

        // second pass: classify cells
        for (const auto & cell : mesh.cells()) {
            const auto & nodes = cell.nodes();

            bool all_above = true;
            bool all_below = true;
            bool any_on_interface = false;

            for (const auto & node : nodes) {
                auto coords = coord_system.coordinates(node->point());
                double y_coord = coords[1];
                double delta = y_coord - y_interface;

                if (std::abs(delta) < tolerance) {
                    any_on_interface = true;
                } else if (delta > 0) {
                    all_below = false;
                } else {
                    all_above = false;
                }
            }

            if (all_above || (any_on_interface && !all_below)) {
                top_mesh.insert(nodes);
            } else if (all_below || (any_on_interface && !all_above)) {
                bottom_mesh.insert(nodes);
            }
            // cells that cross the interface (some above, some below, none on it) are discarded
        }

        // build interface nodes vector
        interface_nodes.reserve(interface_node_set.size());
        interface_nodes.insert(
            interface_nodes.end(), interface_node_set.begin(), interface_node_set.end());

        // sort interface nodes by x-coordinate for consistent ordering
        std::sort(
            interface_nodes.begin(), interface_nodes.end(), [&](const auto & a, const auto & b) {
                return coord_system.coordinates(a->point())[0]
                     < coord_system.coordinates(b->point())[0];
            });

        return { std::move(top_mesh), std::move(bottom_mesh), std::move(interface_nodes) };
    }
}


// end of file
