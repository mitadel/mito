// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class elementT, discretization_t discretizationT>
    struct Discretizer {
        template <
            typename manifoldT, typename constraintsT, typename connectivity_table_type,
            typename map_type, typename constrained_values_type>
        static void apply(
            const manifoldT &, const constraintsT &, connectivity_table_type &, map_type &,
            constrained_values_type &);
    };

    template <class elementT, discretization_t discretizationT>
    auto discretize(
        const auto & manifold, const auto & constraints, auto & connectivity, auto & node_map,
        auto & constrained_values)
    {
        Discretizer<elementT, discretizationT>::apply(
            manifold, constraints, connectivity, node_map, constrained_values);
    }

}


// end of file
