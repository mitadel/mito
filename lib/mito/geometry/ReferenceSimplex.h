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
        // the order of the reference simplex
        constexpr static int order = N;

        // the area of the reference simplex
        constexpr static double area = 1.0 / mito::tensor::factorial<order>();

      public:
        // the type of coordinates in the parametric space
        using parametric_coordinates_type = coordinates_t<N, CARTESIAN>;

        // the function extracting the I component of a parametric point
        template <int I>
        static constexpr auto xi = functions::component<parametric_coordinates_type, I>;
    };

}    // namespace mito


// end of file
