// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::mesh {

    // mesh factory
    template <class cellT>
    auto mesh() -> mesh_t<cellT>;

    // assemble boundary mesh of {mesh}
    template <int N, int D, template <int, int> class cellT>
    auto boundary(const mesh_t<cellT<N, D>> & mesh)
    {
        return Boundary<mesh_t<cellT<N, D>>>::boundary(mesh);
    }

    // get the number of boundary cells of {mesh} (without assembling its boundary mesh)
    template <int N, int D, template <int, int> class cellT>
    auto boundary_size(const mesh_t<cellT<N, D>> & mesh) -> int
    {
        return Boundary<mesh_t<cellT<N, D>>>::boundary_size(mesh);
    }

    // get the mesh of the I-cells that compose {mesh}
    template <int I, int N, int D, template <int, int> class cellT>
    auto filter(const mesh_t<cellT<N, D>> & mesh) -> mesh_t<cellT<I, D>>
    {
        return Filter<mesh_t<cellT<N, D>>, I>::filter(mesh);
    }
}


// end of file
