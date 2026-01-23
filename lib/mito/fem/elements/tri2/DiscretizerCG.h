// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // discretizer specialization for {IsoparametricTriangleP2} with continuous Galerkin
    template <>
    struct Discretizer<IsoparametricTriangleP2, discretization_t::CG> {
        template <
            typename manifoldT, typename constraintsT, typename elements_type, typename map_type,
            typename constrained_nodes_type>
        static void apply(
            const manifoldT & manifold, const constraintsT & constraints, elements_type & elements,
            map_type & node_map, constrained_nodes_type & constrained_nodes)
        {
            // the dimension of the physical space
            constexpr int dim = IsoparametricTriangleP2::dim;

            // assemble the mesh node type
            using mesh_node_type = geometry::node_t<dim>;

            // the discretization node type
            using discretization_node_type =
                typename IsoparametricTriangleP2::discretization_node_type;

            // the connectivity type
            using connectivity_type = typename IsoparametricTriangleP2::connectivity_type;

            // id type of mesh nodes
            using mesh_node_id_t = utilities::index_t<mesh_node_type>;

            // the type of a map between the two vertices and the middle discretization nodes
            using mid_nodes_map_type =
                std::map<std::array<mesh_node_id_t, 2>, discretization_node_type>;

            // create a map to store the mid nodes
            auto mid_nodes_map = mid_nodes_map_type();

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
                auto node_2 =
                    node_map.insert({ nodes[2], discretization_node_type() }).first->second;

                auto ordered_nodes_3 = (node_0.id() < node_1.id()) ?
                                           std::array{ node_0.id(), node_1.id() } :
                                           std::array{ node_1.id(), node_0.id() };
                auto node_3 = mid_nodes_map.insert({ ordered_nodes_3, discretization_node_type() })
                                  .first->second;
                auto ordered_nodes_4 = (node_1.id() < node_2.id()) ?
                                           std::array{ node_1.id(), node_2.id() } :
                                           std::array{ node_2.id(), node_1.id() };
                auto node_4 = mid_nodes_map.insert({ ordered_nodes_4, discretization_node_type() })
                                  .first->second;
                auto ordered_nodes_5 = (node_2.id() < node_0.id()) ?
                                           std::array{ node_2.id(), node_0.id() } :
                                           std::array{ node_0.id(), node_2.id() };
                auto node_5 = mid_nodes_map.insert({ ordered_nodes_5, discretization_node_type() })
                                  .first->second;

                // create a finite element for each cell and add it to the pile
                elements.emplace(
                    cell, coord_system,
                    connectivity_type{ node_0, node_1, node_2, node_3, node_4, node_5 });
            }

            // populate the constrained nodes
            for (const auto & cell : constraints.domain().cells()) {
                for (const auto & node : cell.nodes()) {
                    // get the discretization node associated with the mesh node from the map
                    auto it = node_map.find(node);
                    // add the node to the constrained nodes
                    constrained_nodes.insert(it->second);
                }
                auto node_0 = node_map.at(cell.nodes()[0]);
                auto node_1 = node_map.at(cell.nodes()[1]);
                auto ordered_nodes = (node_0.id() < node_1.id()) ?
                                         std::array{ node_0.id(), node_1.id() } :
                                         std::array{ node_1.id(), node_0.id() };
                auto node = mid_nodes_map.at(ordered_nodes);
                // add the node to the constrained nodes
                constrained_nodes.insert(node);
            }

            // all done
            return;
        }
    };
}

// end of file
