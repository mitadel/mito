// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // simplex alias
    template <int N>
    using simplex_t = oriented_simplex_t<N>;

    // vertex alias
    using vertex_t = unoriented_simplex_t<0>;

    // segment alias
    using segment_t = simplex_t<1>;

    // triangle alias
    using triangle_t = simplex_t<2>;

    // tetrahedron alias
    using tetrahedron_t = simplex_t<3>;
}


#endif

// end of file
