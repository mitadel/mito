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

    // class boundary
    template <class meshT>
    class Boundary;

    // class filter
    template <class meshT, int I>
    class Filter;
}


// end of file
