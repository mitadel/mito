// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // specialization for the polar metric in 2D
    template <>
    struct metric<polar_coordinates_t> {
        static constexpr auto field()
        {
            // the function extracting the x_0 component of a 2D vector
            constexpr auto r = functions::component<polar_coordinates_t, 0>;

            // the function returning the constant e_00 tensor in 2D
            constexpr auto e_rr = fields::uniform_field<polar_coordinates_t>(tensor::e_00<2>);
            // the function returning the constant e_11 tensor in 2D
            constexpr auto e_tt = fields::uniform_field<polar_coordinates_t>(tensor::e_11<2>);

            // return the metric field in polar coordinates
            return e_rr + functions::pow<2>(r) * e_tt;
        }
    };
}


// end of file
