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

}


// end of file
