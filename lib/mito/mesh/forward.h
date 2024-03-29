// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <class cellT>
    class Mesh;

    // mesh alias
    template <class cellT>
    using mesh_t = Mesh<cellT>;
}


#endif

// end of file
