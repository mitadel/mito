// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_polar_coordinate_basis_h)
#define mito_geometry_polar_coordinate_basis_h


namespace mito::geometry {

    // polar coordinates type
    using polar_coordinates_t = coordinates_t<2, POLAR>;

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
                return fields::uniform_field<polar_coordinates_t>(mito::e_0<2>);
            }

            if constexpr (I == 1) {
                // e_theta
                return _r * fields::uniform_field<polar_coordinates_t>(mito::e_1<2>);
            }
        }
    };
}


#endif

// end of file
