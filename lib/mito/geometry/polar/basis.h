// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // polar coordinate basis in 2D
    template <>
    struct basis<polar_coordinates_t> {

      private:
        // the function extracting the 0th component
        static constexpr auto _r = functions::component<polar_coordinates_t, 0>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < 2)
        static constexpr auto e()
        {
            if constexpr (I == 0) {
                // return e_r
                return functions::constant<polar_coordinates_t>(mito::tensor::e_0<2>);
            }

            if constexpr (I == 1) {
                // e_theta
                return _r * functions::constant<polar_coordinates_t>(mito::tensor::e_1<2>);
            }
        }
    };
}


// end of file
