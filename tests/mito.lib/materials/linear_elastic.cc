// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/materials.h>


constexpr mito::real eps = 0.1;
constexpr mito::real tol = 1.e-3;

auto
numerical_tangent(const auto & material, const mito::matrix_c auto & Du)
    -> mito::fourth_order_tensor_t<3>
{
    // take note of the original deformation tensor
    auto u = mito::vector_t<3>();
    auto Du_perturbed = Du;

    auto P_plus = mito::matrix_t<3>();
    auto P_minus = mito::matrix_t<3>();
    auto C_numerical = mito::fourth_order_tensor_t<3>();

    auto C_plus = mito::fourth_order_tensor_t<3>();
    auto C_minus = mito::fourth_order_tensor_t<3>();

    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            // positive perturbation in direction {i, j}
            Du_perturbed[{ k, l }] = Du[{ k, l }] + eps;
            // compute {P_plus}
            material.Constitutive(u, Du_perturbed, P_plus, C_plus);

            // negative perturbation in direction {i, j}
            Du_perturbed[{ k, l }] = Du[{ k, l }] - eps;
            // compute {P_minus}
            material.Constitutive(u, Du_perturbed, P_minus, C_minus);

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    C_numerical[{ i, j, k, l }] =
                        (P_plus[{ i, j }] - P_minus[{ i, j }]) / (2.0 * eps);
                }
            }

            // back to the original state {i, j}
            Du_perturbed[{ k, l }] = Du[{ k, l }];
        }
    }

    // all done
    return C_numerical;
}


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

    // the undeformed state
    auto u = mito::vector_t<3>();
    // TOFIX: cannot do it with the line below as then Du is diagonal. Then operator+= is illegal
    // and component-wise += will lead to the wrong result
    // same thing for P
    // auto Du = mito::identity<3>;
    auto Du = mito::matrix_t<3>() + mito::identity<3>;

    // perturb the state
    const auto perturbation = mito::real(0.1);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Du[{ i, j }] += perturbation * (2.0 * (mito::real) rand() / RAND_MAX - 1.0);
        }
    }

    auto P = mito::matrix_t<3>();
    auto C = mito::fourth_order_tensor_t<3>();
    material.Constitutive(u, Du, P, C);

    auto C_numerical = numerical_tangent(material, Du);
    auto C_diff = C - C_numerical;
    channel << "C - C_numerical = " << mito::norm(C_diff) << journal::endl;

    // expect a reasonable match between the analytical and the numerical tangents
    EXPECT_NEAR(mito::norm(C_diff), 0.0, tol);
}
