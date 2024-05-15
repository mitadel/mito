// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_spherical_coordinates_h)
#define mito_geometry_algebra_spherical_coordinates_h


namespace mito::geometry {

    // addition of spherical coordinates {A} and vector {v} stemming from point A
    template <>
    constexpr auto operator+(const Coordinates<3, SPHERICAL> & A, const vector_t<3> & v)
        -> Coordinates<3, SPHERICAL>
    {
        // get r, theta and phi of point A
        const auto & r_A = A[0];
        const auto & theta_A = A[1];
        const auto & phi_A = A[2];

        // precompute handful variables
        const auto sin_theta_A = std::sin(theta_A);
        const auto cos_theta_A = std::cos(theta_A);

        // get the r, theta and phi components of vector {v}
        const auto & v_r = v[0];
        const auto v_theta = v[1] / r_A;
        const auto v_phi = v[2] / (r_A * sin_theta_A);

        // compute phi of point B, which is point A plus vector {v}
        const auto phi_B =
            phi_A
            + std::atan2(
                v_phi * r_A * sin_theta_A, (r_A + v_r) * sin_theta_A + v_theta * r_A * cos_theta_A);

        // precompute handful variables
        const auto sin_phi_A_B = std::sin(phi_B - phi_A);
        const auto cos_phi_A_B = std::cos(phi_B - phi_A);

        // compute theta of point B, which is point A plus vector {v}
        const auto theta_B = std::atan2(
            v_phi * r_A * sin_theta_A * sin_phi_A_B
                + (v_theta * r_A * cos_theta_A + (r_A + v_r) * sin_theta_A) * cos_phi_A_B,
            -v_theta * r_A * sin_theta_A + (r_A + v_r) * cos_theta_A);

        // precompute handful variables
        const auto sin_theta_B = std::sin(theta_B);
        const auto cos_theta_B = std::cos(theta_B);

        // compute r of point B, which is point A plus vector {v}
        const auto r_B =
            (v_r + r_A) * (sin_theta_A * sin_theta_B * cos_phi_A_B + cos_theta_A * cos_theta_B)
            + v_theta * r_A * (cos_theta_A * sin_theta_B * cos_phi_A_B - sin_theta_A * cos_theta_B)
            + v_phi * r_A * sin_theta_A * sin_theta_B * sin_phi_A_B;

        // all done
        return Coordinates<3, SPHERICAL>({ r_B, theta_B, phi_B });
    }

    // subtraction of coordinates B - A, returns a vector stemming from A
    template <>
    constexpr auto operator-(
        const Coordinates<3, SPHERICAL> & B, const Coordinates<3, SPHERICAL> & A)
        -> mito::vector_t<3>
    {
        // get the radius and angle of the two points
        const auto & r_A = A[0];
        const auto & r_B = B[0];
        const auto & theta_A = A[1];
        const auto & theta_B = B[1];
        const auto & phi_A = A[2];
        const auto & phi_B = B[2];

        const auto sin_theta_A = std::sin(theta_A);
        const auto sin_theta_B = std::sin(theta_B);
        const auto cos_theta_A = std::cos(theta_A);
        const auto cos_theta_B = std::cos(theta_B);
        const auto cos_phi_A_B = std::cos(phi_B - phi_A);
        const auto sin_phi_A_B = std::sin(phi_B - phi_A);

        return { r_B * (sin_theta_A * sin_theta_B * cos_phi_A_B + cos_theta_A * cos_theta_B) - r_A,
                 r_B * (sin_theta_B * cos_theta_A * cos_phi_A_B - sin_theta_A * cos_theta_B),
                 r_B * sin_theta_B * sin_phi_A_B };
    }
}


#endif

// end of file
