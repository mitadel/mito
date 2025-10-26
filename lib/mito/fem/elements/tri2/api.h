// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    auto discretize_triangleP2_CG(
        const manifoldT & manifold, const constraintsT & constraints, auto & elements,
        auto & node_map, auto & constrained_nodes)
    {
        return DiscretizeTriangle2CG<constraintsT>::template discretize<manifoldT>(
            manifold, constraints, elements, node_map, constrained_nodes);
    }
}


// end of file
