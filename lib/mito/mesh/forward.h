// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::mesh {

    // class mesh
    template <class cellT>
    class Mesh;

    // mesh alias
    template <class cellT>
    using mesh_t = Mesh<cellT>;

    // concept of a mesh
    template <class F>
    concept mesh_c = requires(F c) {
        // require that F only binds to {Mesh} specializations
        []<class cellT>(const mesh_t<cellT> &) {
        }(c);
    };

    // class boundary
    template <class meshT>
    class Boundary;

    // class filter
    template <class meshT, int I>
    class Filter;
}


// end of file
