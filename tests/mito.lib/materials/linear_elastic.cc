// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/materials.h>


// the perturbation for computing the numerical derivatives
constexpr mito::real eps = 1.e-5;
// the tolerance for passing the test
constexpr mito::real tol = 1.e-15;


TEST(LinearElastic, TestLinearElastic)
{
    // material parameters
    const auto rho = mito::real(1.0);
    const auto E = mito::real(1.0);
    const auto nu = mito::real(0.3);

    // a linear elastic material
    auto material = mito::materials::linear_elastic(rho, E, nu);

    // the material consistency test
    auto test = mito::materials::consistency_test(material, eps, tol);

    // choose a deformation gradient as a random perturbation of amplitude 0.1 around the identity
    auto F = mito::matrix_t<3>(mito::identity<3> + mito::random<mito::matrix_t<3>>(0.1));

    // test that the stress tensor is consistent with the strain energy density
    EXPECT_TRUE(test.test_stress(F));

    // test that the tangent tensor is consistent with the stress tensor
    EXPECT_TRUE(test.test_tangents(F));
}
