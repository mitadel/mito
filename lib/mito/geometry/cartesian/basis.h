// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_cartesian_coordinate_basis_h)
#define mito_geometry_cartesian_coordinate_basis_h


namespace mito::geometry {

    // cartesian coordinate basis in dimension D
    template <int D>
    struct basis<coordinates_t<D, CARTESIAN>> {

      private:
        // the type of coordinates
        using coordinates_type = coordinates_t<D, CARTESIAN>;

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e()
        {
            return fields::uniform_field<coordinates_type>(mito::e<I, D>);
        }
    };
}


#endif

// end of file
