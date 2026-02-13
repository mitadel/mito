// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // manifold alias
    template <class cellT, geometry::coordinates_c coordsT, class metricFieldT>
    using manifold_t = Manifold<cellT, coordsT, metricFieldT>;

    // factory of manifolds from a mesh and a coordinate system
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system);

    // factory of manifolds with explicit metric field
    template <class cellT, geometry::coordinates_c coordsT, class metricFieldT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const metricFieldT & metric_field);
}


// end of file
