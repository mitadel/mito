// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/materials.h>


constexpr mito::real tol = 1.e-15;


TEST(LinearElastic, TestLinearElastic)
{
    // make a channel
    journal::info_t channel("tests.linear_elastic");

    // material parameters
    const auto rho = mito::real(1000);
    const auto E = mito::real(210e9);
    const auto nu = mito::real(0.33);

    // a linear elastic material
    auto material = mito::materials::linear_elastic(rho, E, nu);

    // choose a deformation gradient as a random perturbation of amplitude 0.1 around the identity
    auto F = mito::identity<3> + mito::random<mito::matrix_t<3>>(0.1);

    // compute the analytical stress
    auto P_analytical = material.stress(F);

    // compute the numerical stress
    auto P_numerical = mito::materials::numerical_stress(material, F);

    // print the errors
    channel << "Relative error for the stress tensor: "
            << mito::norm(P_analytical - P_numerical) / mito::norm(P_analytical) << journal::endl;

    // expect a reasonable match between the analytical and the numerical stresses
    EXPECT_NEAR(mito::norm(P_analytical - P_numerical) / mito::norm(P_analytical), 0.0, tol);

    // compute the analytical tangent
    auto C_analytical = material.tangents(F);

    // compute the numerical tangent
    auto C_numerical = mito::materials::numerical_tangent(material, F);

    channel << "Relative error for the tangent tensor: "
            << mito::norm(C_analytical - C_numerical) / mito::norm(C_analytical) << journal::endl;

    // expect a reasonable match between the analytical and the numerical tangents
    EXPECT_NEAR(mito::norm(C_analytical - C_numerical) / mito::norm(C_analytical), 0.0, tol);
}
