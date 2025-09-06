// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <int N>
    class ReferenceSimplex {

      public:
        // the barycentric coordinates type
        using barycentric_coordinates_type = geometry::coordinates_t<N, geometry::CARTESIAN>;

        // the function extracting the I component of a parametric point
        template <int I>
        static constexpr auto xi =
            fields::field(functions::component<barycentric_coordinates_type, I>);
    };

}    // namespace mito


// end of file
