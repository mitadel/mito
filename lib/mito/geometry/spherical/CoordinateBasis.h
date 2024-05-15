// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_spherical_coordinate_basis_h)
#define mito_geometry_spherical_coordinate_basis_h


namespace mito::geometry {

    // spherical coordinate basis in 3D
    template <>
    class Basis<coordinates_t<3, SPHERICAL>> {

        // the type of coordinates
        using coordinates_type = coordinates_t<3, SPHERICAL>;

        // the function extracting the 0th component
        static constexpr auto _r = functions::component<coordinates_type, 0>;

        // the function extracting the 1st component
        static constexpr auto _theta = functions::component<coordinates_type, 1>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < 3)
        static constexpr auto e()
        {
            if constexpr (I == 0) {
                // return e_r
                return fields::uniform_field<coordinates_type>(mito::e_0<3>);
            }

            if constexpr (I == 1) {
                // return e_theta
                return _r * fields::uniform_field<coordinates_type>(mito::e_1<3>);
            }

            if constexpr (I == 2) {
                // return e_phi
                return _r * functions::sin(_theta)
                     * fields::uniform_field<coordinates_type>(mito::e_2<3>);
            }
        }
    };
}


#endif

// end of file
