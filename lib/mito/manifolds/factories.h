// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, geometry::CoordinateType coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<cellT::dim, coordsT> & coordinate_system)
        -> manifold_t<cellT, coordsT>
    {
        return manifold_t<cellT, coordsT>(mesh, coordinate_system);
    }

    // factory submanifolds
    template <class cellT, geometry::CoordinateType coordsT, class volumeFormT>
    constexpr auto submanifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<cellT::dim, coordsT> & coordinate_system,
        volumeFormT volume_form) -> submanifold_t<cellT, coordsT, volumeFormT>
    {
        return submanifold_t<cellT, coordsT, volumeFormT>(mesh, coordinate_system, volume_form);
    }
}


#endif

// end of file
