// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_polar_Metric_h)
#define mito_geometry_polar_Metric_h


namespace mito::geometry {

    // specialization for the polar metric in 2D
    template <>
    struct metric<coordinates_t<2, POLAR>> {
        static constexpr auto field()
        {
            // the type of coordinates
            using coordinates_t = coordinates_t<2, POLAR>;

            // the function extracting the x_0 component of a 2D vector
            constexpr auto r = functions::component<coordinates_t, 0>;

            // the function returning the constant e_00 tensor in 2D
            constexpr auto e_rr = fields::uniform_field<coordinates_t>(e_00<2>);
            // the function returning the constant e_11 tensor in 2D
            constexpr auto e_tt = fields::uniform_field<coordinates_t>(e_11<2>);

            // return the metric field in polar coordinates
            return e_rr + functions::pow<2>(r) * e_tt;
        }
    };
}


#endif

// end of file
