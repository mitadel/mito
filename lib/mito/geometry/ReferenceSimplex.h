// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
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

      private:
        // helper to compute 1 - xi0 - xi1 - ... - xi(N-1)
        template <int... a>
        static constexpr auto _one_minus_xis(tensor::integer_sequence<a...>)
        {
            return 1.0 - (functions::component<parametric_coordinates_type, a> + ...);
        }

      public:
        // the type of coordinates in the parametric space
        using parametric_coordinates_type = coordinates_t<N, CARTESIAN>;

        // the parametric coordinates
        template <int I>
        static constexpr auto xi = [] {
            static_assert(I >= 0 && I <= N);
            if constexpr (I == 0) {
                // the complementary of the sum of all parametric coordinates
                return _one_minus_xis(tensor::make_integer_sequence<N>{});
            } else {
                // (I-1)-th parametric coordinate
                return functions::component<parametric_coordinates_type, I - 1>;
            }
        }();
    };

}    // namespace mito


// end of file
