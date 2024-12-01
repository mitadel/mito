// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/materials.h>


TEST(Gent, TestGent)
{
    // make a channel
    journal::info_t channel("tests.gent");

    // material parameters
    const auto rho = mito::real(1.0e3);
    const auto kappa = mito::real(1.5e9);
    const auto mu = mito::real(5.0e6);
    const auto Jm = mito::real(50);

    // a gent material
    auto material = mito::materials::gent(rho, kappa, mu, Jm);

    // choose a deformation gradient as a random perturbation of amplitude 0.1 around the identity
    auto Du = mito::identity<3> + mito::random<mito::matrix_t<3>>(0.1);

    // compute the analytical stress
    auto P_analytical = material.stress(Du);

    // compute the numerical stress
    auto P_numerical = mito::materials::numerical_stress(material, Du, 1.e-4);

    // print the errors
    channel << "Relative error for the stress tensor: "
            << mito::norm(P_analytical - P_numerical) / mito::norm(P_analytical) << journal::endl;

    // expect a reasonable match between the analytical and the numerical stresses
    EXPECT_NEAR(mito::norm(P_analytical - P_numerical) / mito::norm(P_analytical), 0.0, 1.e-7);

    // compute the analytical tangent
    auto C_analytical = material.tangents(Du);

    // compute the numerical tangent
    auto C_numerical = mito::materials::numerical_tangent(material, Du, 1.e-4);

    // print the errors
    channel << "Relative error for the tangent tensor: "
            << mito::norm(C_analytical - C_numerical) / mito::norm(C_analytical) << journal::endl;

    // expect a reasonable match between the analytical and the numerical tangents
    EXPECT_NEAR(mito::norm(C_analytical - C_numerical) / mito::norm(C_analytical), 0.0, 1.e-6);
}
