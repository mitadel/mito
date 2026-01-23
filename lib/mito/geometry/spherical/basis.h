// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // spherical coordinate basis in 3D
    template <>
    struct basis<spherical_coordinates_t> {

      private:
        // the function extracting the 0th component
        static constexpr auto _r = functions::component<spherical_coordinates_t, 0>;

        // the function extracting the 1st component
        static constexpr auto _theta = functions::component<spherical_coordinates_t, 1>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < 3)
        static constexpr auto e()
        {
            if constexpr (I == 0) {
                // return e_r
                return functions::constant<spherical_coordinates_t>(mito::tensor::e_0<3>);
            }

            if constexpr (I == 1) {
                // return e_theta
                return _r * functions::constant<spherical_coordinates_t>(mito::tensor::e_1<3>);
            }

            if constexpr (I == 2) {
                // return e_phi
                return _r * functions::sin(_theta)
                     * functions::constant<spherical_coordinates_t>(mito::tensor::e_2<3>);
            }
        }
    };
}


// end of file
