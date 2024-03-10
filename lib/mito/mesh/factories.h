// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // mesh factory
    template <class cellT, int D>
    auto mesh(geometry::geometry_t<D> & geometry) -> mesh_t<cellT, D>
    {
        return mesh_t<cellT, D>(geometry);
    }

}


#endif

// end of file
