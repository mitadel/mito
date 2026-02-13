// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // factory manifold with explicit metric field
    template <class cellT, geometry::coordinates_c coordsT, class metricFieldT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const metricFieldT & metric_field)
    {
        return manifold_t<cellT, coordsT, metricFieldT>(mesh, coordinate_system, metric_field);
    }

    // factory of manifolds from a mesh and a coordinate system (works for any dimension)
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system)
    {
        constexpr auto g = geometry::metric<coordsT>::field();
        return manifold(mesh, coordinate_system, g);
    }
}


// end of file
