// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::summit {

    // the summit type of a geometric simplex
    template <geometry::geometric_simplex_c cellT>
    struct cell;

    // specialization for a geometric segment
    template <geometry::geometric_segment_c cellT>
    struct cell<cellT> {
        static constexpr int type = 2;
    };

    // specialization for a geometric triangle
    template <geometry::geometric_triangle_c cellT>
    struct cell<cellT> {
        static constexpr int type = 3;
    };

    // specialization for a geometric tetrahedron
    template <geometry::geometric_tetrahedron_c cellT>
    struct cell<cellT> {
        static constexpr int type = 4;
    };

}    // namespace io::summit


// end of file
