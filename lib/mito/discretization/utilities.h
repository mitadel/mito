// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // populate a container with a collection of all nodes in a function space
    template <class functionSpaceT, class nodesCollectionT>
    inline auto get_nodes(const functionSpaceT & function_space, nodesCollectionT & nodes) -> void
    {
        for (const auto & element : function_space.elements()) {
            for (const auto & node : element.discretization_nodes()) {
                nodes.insert(node);
            }
        }

        // all done
        return;
    }

}


// end of file
