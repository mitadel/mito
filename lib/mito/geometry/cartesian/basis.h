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
    struct basis<cartesian_coordinates_t<D>> {

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e()
        {
            return fields::uniform_field<cartesian_coordinates_t<D>>(mito::e<I, D>);
        }
    };
}


#endif

// end of file
