// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_Metric_h)
#define mito_geometry_Metric_h


namespace mito::geometry {

    // the metric tensor field in {coordsT} coordinates
    template <coordinates_c coordsT>
    struct metric {};

    // specialization for the Euclidean metric
    template <int D>
    struct metric<coordinates_t<D, CARTESIAN>> {
        static constexpr auto field()
        {
            // the type of coordinates
            using coordinates_t = coordinates_t<D, CARTESIAN>;

            // return the identity field
            return fields::identity_tensor_field<coordinates_t, D>;
        }
    };

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
            return e_rr + mito::functions::pow<2>(r) * e_tt;
        }
    };

    // specialization for the spherical metric in 3D
    template <>
    struct metric<coordinates_t<3, SPHERICAL>> {
        static constexpr auto field()
        {
            // the type of coordinates
            using coordinates_t = coordinates_t<3, SPHERICAL>;

            // the function extracting the x_0 component of a 3D vector
            constexpr auto r = functions::component<coordinates_t, 0>;

            // the function extracting the x_1 component of a 3D vector
            constexpr auto t = functions::component<coordinates_t, 1>;

            // the function returning the constant e_00 tensor in 3D
            constexpr auto e_rr = fields::uniform_field<coordinates_t>(e_00<3>);
            // the function returning the constant e_11 tensor in 3D
            constexpr auto e_tt = fields::uniform_field<coordinates_t>(e_11<3>);
            // the function returning the constant e_11 tensor in 3D
            constexpr auto e_pp = fields::uniform_field<coordinates_t>(e_22<3>);

            // return the metric field in spherical coordinates
            return e_rr + mito::functions::pow<2>(r) * e_tt
                 + mito::functions::pow<2>(r * mito::functions::sin(t)) * e_pp;
        }
    };
}


#endif

// end of file
