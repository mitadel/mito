// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifold_coordinate_bases_h)
#define mito_manifold_coordinate_bases_h


namespace mito::manifolds {

    // the basis for vectors in {coordsT} coordinates
    template <geometry::coordinates_c coordsT>
    class Basis {};

    // cartesian coordinate basis in dimension D
    template <int D>
    class Basis<geometry::coordinates_t<D, geometry::CARTESIAN>> {

        // the type of coordinates
        using coordinates_type = geometry::coordinates_t<D, geometry::CARTESIAN>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e()
        {
            return fields::uniform_field<coordinates_type>(mito::e<I, D>);
        }
    };

    // polar coordinate basis in 2D
    template <>
    class Basis<geometry::coordinates_t<2, geometry::POLAR>> {

        // the type of coordinates
        using coordinates_type = geometry::coordinates_t<2, geometry::POLAR>;

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
                return mito::fields::uniform_field<coordinates_type>(mito::e_0<2>);
            }

            if constexpr (I == 1) {
                // e_theta
                return _r * mito::fields::uniform_field<coordinates_type>(mito::e_1<2>);
            }
        }
    };

    // spherical coordinate basis in 3D
    template <>
    class Basis<geometry::coordinates_t<3, geometry::SPHERICAL>> {

        // the type of coordinates
        using coordinates_type = geometry::coordinates_t<3, geometry::SPHERICAL>;

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
                return mito::fields::uniform_field<coordinates_type>(mito::e_0<3>);
            }

            if constexpr (I == 1) {
                // return e_theta
                return _r * mito::fields::uniform_field<coordinates_type>(mito::e_1<3>);
            }

            if constexpr (I == 2) {
                // return e_phi
                return _r * mito::functions::sin(_theta)
                     * mito::fields::uniform_field<coordinates_type>(mito::e_2<3>);
            }
        }
    };
}


#endif

// end of file
