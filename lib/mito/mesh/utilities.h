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
}


// end of file
