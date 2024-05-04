// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <class cellT, geometry::coordinates_c coordsT>
    using manifold_t = Manifold<cellT, coordsT>;

    // submanifold alias
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    using submanifold_t = Submanifold<cellT, coordsT, volumeFormT>;

    // factory manifolds
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system)
        -> manifold_t<cellT, coordsT>;

    // factory submanifolds
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    constexpr auto submanifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system, volumeFormT volume_form)
        -> submanifold_t<cellT, coordsT, volumeFormT>;
}


#endif

// end of file
