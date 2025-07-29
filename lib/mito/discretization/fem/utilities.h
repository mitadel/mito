// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // populate a container with a collection of all nodes in a function space
    template <function_space_c functionSpaceT, class nodesCollectionT>
    inline auto get_discretization_nodes(
        const functionSpaceT & function_space, nodesCollectionT & nodes) -> void
    {
        for (const auto & element : function_space.elements()) {
            for (const auto & node : element.connectivity()) {
                nodes.insert(node);
            }
        }

        // all done
        return;
    }

    // populate a container with a collection of all first order nodes in a function space (include
    // only the mesh nodes, exclude the nodes of the subconnectivities)
    template <function_space_c functionSpaceT, class nodesCollectionT>
    inline auto get_mesh_discretization_nodes(
        const functionSpaceT & function_space, nodesCollectionT & nodes) -> void
    {
        for (const auto & element : function_space.elements()) {
            for (const auto & node : element.geometric_simplex().nodes()) {
                nodes.insert(function_space.node_map().at(node));
            }
        }

        // all done
        return;
    }
}


// end of file
