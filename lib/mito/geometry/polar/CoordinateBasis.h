// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_polar_coordinate_basis_h)
#define mito_geometry_polar_coordinate_basis_h


namespace mito::geometry {

    // polar coordinate basis in 2D
    template <>
    class Basis<coordinates_t<2, POLAR>> {

        // the type of coordinates
        using coordinates_type = coordinates_t<2, POLAR>;

        // the function extracting the 0th component
        static constexpr auto _r = functions::component<coordinates_type, 0>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < 2)
        static constexpr auto e()
        {
            if constexpr (I == 0) {
                // return e_r
                return fields::uniform_field<coordinates_type>(mito::e_0<2>);
            }

            if constexpr (I == 1) {
                // e_theta
                return _r * fields::uniform_field<coordinates_type>(mito::e_1<2>);
            }
        }
    };
}


#endif

// end of file
