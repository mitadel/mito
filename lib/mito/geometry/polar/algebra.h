// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_polar_coordinates_h)
#define mito_geometry_algebra_polar_coordinates_h


namespace mito::geometry {

    // addition of polar coordinates {A} and vector {v} stemming from point A
    template <>
    constexpr auto operator+(const polar_coordinates_t & A, const vector_t<2> & v)
        -> polar_coordinates_t
    {
        // get r and theta of point A
        const auto & r_A = A[0];
        const auto & theta_A = A[1];

        // evaluate {e_r} and {e_theta} at A
        const auto e_r = polar::e_r(A);
        const auto e_theta = polar::e_theta(A);

        // get the radial and hoop component of vector {v}
        const auto v_r = v * e_r;
        const auto v_theta = v * e_theta / (e_theta * e_theta);

        // compute r and theta of point B, which is point A plus vector {v}
        auto r_B = std::sqrt((r_A + v_r) * (r_A + v_r) + v_theta * r_A * v_theta * r_A);
        auto theta_B = theta_A + std::atan2(v_theta * r_A, r_A + v_r);

        // all done
        return polar_coordinates_t({ r_B, theta_B });
    }

    // subtraction of coordinates B - A, returns a vector stemming from A
    template <>
    constexpr auto operator-(const polar_coordinates_t & B, const polar_coordinates_t & A)
        -> mito::vector_t<2>
    {
        // get the radius and angle of the two points
        const auto & r_A = A[0];
        const auto & r_B = B[0];
        const auto & theta_A = A[1];
        const auto & theta_B = B[1];

        return (r_B * std::cos(theta_B - theta_A) - r_A) * polar::e_r(A)
             + (r_B * std::sin(theta_B - theta_A)) * polar::e_theta(A) / r_A;
    }
}


#endif

// end of file
