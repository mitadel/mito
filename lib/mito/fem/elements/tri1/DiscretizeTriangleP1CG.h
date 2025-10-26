// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <constraints::constraint_c constraintsT>
    class DiscretizeTriangleP1CG {

      private:
        // the element type
        using element_type = IsoparametricTriangleP1;
        // the constraints type
        using constraints_type = constraintsT;
        // the function space type
        using function_space_type = FunctionSpace<element_type, constraints_type>;
        // the mesh node type
        using mesh_node_type = typename function_space_type::mesh_node_type;
        // the discretization node type
        using discretization_node_type = typename function_space_type::discretization_node_type;
        // the elements type
        using elements_type = typename function_space_type::elements_type;
        // the nodes type
        using connectivity_type = typename element_type::connectivity_type;
        // the constrained nodes type
        using constrained_nodes_type = typename function_space_type::constrained_nodes_type;
        // the type of a map between the mesh nodes and discretization nodes
        using map_type = typename function_space_type::map_type;

      public:
        // discretize {manifold} with first order isoparametric triangles subject to {constraints}
        // this method creates three data structures representing the discretization: a collection
        // of discrete elements, a map between the nodes in a mesh and the discretization nodes, and
        // a set of constrained nodes
        template <manifolds::manifold_c manifoldT>
        // require compatibility between the manifold cell and the finite element cell
        requires(std::is_same_v<
                 typename manifoldT::mesh_type::cell_type, typename element_type::cell_type>)
        static void discretize(
            const manifoldT & manifold, const constraints_type & constraints,
            elements_type & elements, map_type & node_map,
            constrained_nodes_type & constrained_nodes)
        {

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

                // create a finite element for each cell and add it to the pile
                elements.emplace(cell, coord_system, connectivity_type{ node_0, node_1, node_2 });
            }

            // populate the constrained nodes
            for (const auto & cell : constraints.domain().cells()) {
                for (const auto & node : cell.nodes()) {
                    // get the discretization node associated with the mesh node from the map
                    auto it = node_map.find(node);
                    // add the node to the constrained nodes
                    constrained_nodes.insert(it->second);
                }
            }

            // all done
            return;
        }
    };
}

// end of file
