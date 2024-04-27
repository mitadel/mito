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

}


#endif

// end of file
