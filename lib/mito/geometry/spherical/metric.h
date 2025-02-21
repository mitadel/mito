// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // specialization for the spherical metric in 3D
    template <>
    struct metric<spherical_coordinates_t> {
        static constexpr auto field()
        {
            // the function extracting the x_0 component of a 3D vector
            constexpr auto r = functions::component<spherical_coordinates_t, 0>;

            // the function extracting the x_1 component of a 3D vector
            constexpr auto t = functions::component<spherical_coordinates_t, 1>;

            // the function returning the constant e_00 tensor in 3D
            constexpr auto e_rr = fields::uniform_field<spherical_coordinates_t>(tensor::e_00<3>);
            // the function returning the constant e_11 tensor in 3D
            constexpr auto e_tt = fields::uniform_field<spherical_coordinates_t>(tensor::e_11<3>);
            // the function returning the constant e_11 tensor in 3D
            constexpr auto e_pp = fields::uniform_field<spherical_coordinates_t>(tensor::e_22<3>);

            // return the metric field in spherical coordinates
            return e_rr + functions::pow<2>(r) * e_tt
                 + functions::pow<2>(r * functions::sin(t)) * e_pp;
        }
    };
}


// end of file
