// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class elementT, discretization_t discretizationT>
    struct Discretizer {
        template <
            typename manifoldT, typename constraintsT, typename elements_type, typename map_type,
            typename constrained_nodes_type>
        static void apply(
            const manifoldT &, const constraintsT &, elements_type &, map_type &,
            constrained_nodes_type &);
    };

    template <class elementT, discretization_t discretizationT>
    auto discretize(
        const auto & manifold, const auto & constraints, auto & elements, auto & node_map,
        auto & constrained_nodes)
    {
        Discretizer<elementT, discretizationT>::apply(
            manifold, constraints, elements, node_map, constrained_nodes);
    }

}


// end of file
