// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // discretizer specialization for {IsoparametricSegmentP1} with continuous Galerkin
    template <>
    struct Discretizer<IsoparametricSegmentP1, discretization_t::CG> {
        template <
            typename manifoldT, typename constraintsT, typename elements_type, typename map_type,
            typename constrained_nodes_type>
        static void apply(
            const manifoldT & manifold, const constraintsT & constraints, elements_type & elements,
            map_type & node_map, constrained_nodes_type & constrained_nodes)
        {

            // the discretization node type
            using discretization_node_type =
                typename IsoparametricSegmentP1::discretization_node_type;

            // the connectivity type
            using connectivity_type = typename IsoparametricSegmentP1::connectivity_type;

            // get the coordinate system of the manifold
            const auto & coord_system = manifold.coordinate_system();

            // loop on the cells of the mesh
            for (const auto & cell : manifold.elements()) {

                // get the nodes of the cell
                const auto & nodes = cell.nodes();

                // add the nodes to the map (if the mesh node is already present in the map,
                // then the present discretization node is used)
                auto node_0 =
                    node_map.insert({ nodes[0], discretization_node_type() }).first->second;
                auto node_1 =
                    node_map.insert({ nodes[1], discretization_node_type() }).first->second;

                // create a finite element for each cell and add it to the pile
                elements.emplace(cell, coord_system, connectivity_type{ node_0, node_1 });
            }

            // populate the constrained nodes
            // In 1D, constraints.domain() is a set of nodes, not a mesh with cells, so we loop on
            // the nodes directly
            for (const auto & node : constraints.domain()) {
                // get the discretization node associated with the mesh node from the map
                auto it = node_map.find(node);
                // add the node to the constrained nodes
                constrained_nodes.insert(it->second);
            }

            // all done
            return;
        }
    };
}

// end of file
