// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    using manifold_t = Manifold<cellT, coordsT, volumeFormT>;

    // factory of manifolds from a mesh and a coordinate system
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system);

    // factory submanifold from a mesh, a coordinate system and set of normal fields
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    constexpr auto submanifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const mito::fields::vector_field_c auto & normal_field);
}


// end of file
