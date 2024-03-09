// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <class cellT>
    auto mesh() -> mesh_t<cellT>;


    //
    template <int N, int D, template <int, int> class cellT>
    auto boundary(const mesh_t<cellT<N, D>> & mesh) -> mesh_t<cellT<N - 1, D>>
    {
        return Boundary<mesh_t<cellT<N, D>>>::boundary(mesh);
    }

    //
    template <int N, int D, template <int, int> class cellT>
    auto boundary_size(const mesh_t<cellT<N, D>> & mesh) -> int
    {
        return Boundary<mesh_t<cellT<N, D>>>::boundary_size(mesh);
    }
}


#endif

// end of file
