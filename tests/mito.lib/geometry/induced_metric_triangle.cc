// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <cmath>


using coordinates_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coordinates_3d_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;
using parametric_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


// helper function to compute directional derivative: grad · J_col
// this computes the inner product of the gradient with a tangent vector (column of Jacobian)
template <int AMBIENT_DIM, int PARAM_DIM>
double
directional_derivative(
    const mito::tensor::vector_t<AMBIENT_DIM> & grad,
    const mito::tensor::matrix_t<AMBIENT_DIM, PARAM_DIM> & J, int col_index)
{
    double result = 0.0;
    for (int i = 0; i < AMBIENT_DIM; ++i) {
        result += grad[i] * J[{ i, col_index }];
    }
    return result;
}

// helper function to test gradient in all parametric directions
// checks that ⟨grad, tangent_i⟩ = dphi[i] for each parametric direction i
template <int AMBIENT_DIM, int PARAM_DIM>
void
test_gradient_directions(
    const mito::tensor::vector_t<PARAM_DIM> & dphi,
    const mito::tensor::vector_t<AMBIENT_DIM> & grad,
    const mito::tensor::matrix_t<AMBIENT_DIM, PARAM_DIM> & J)
{
    for (int col = 0; col < PARAM_DIM; ++col) {
        EXPECT_DOUBLE_EQ(dphi[col], directional_derivative(grad, J, col));
    }
}


TEST(Geometry, InducedMetricTriangle2D)
{
    // unit right triangle: vertices (0,0), (1,0), (0,1)
    // parametrization: x = (xi1 - xi0, 1 - xi0), J = [[-1, 1], [-1, 0]]
    // this gives g_induced = J^T * J = [[2, -1], [-1, 1]]
    auto jacobian = [](const parametric_2d_t &) {
        mito::tensor::matrix_t<2, 2> J;
        J[{ 0, 0 }] = -1;
        J[{ 0, 1 }] = 1;
        J[{ 1, 0 }] = -1;
        J[{ 1, 1 }] = 0;
        return J;
    };
    auto parametrization = [](const parametric_2d_t & xi) {
        return mito::geometry::cartesian::coordinates({ xi[1] - xi[0], 1.0 - xi[0] });
    };

    auto metric = mito::geometry::induced_metric<2, coordinates_2d_t, parametric_2d_t>(
        jacobian, parametrization);

    auto xi = mito::geometry::cartesian::coordinates<2>({ 1.0 / 3.0, 1.0 / 3.0 });
    auto g_val = metric.g(xi);
    EXPECT_DOUBLE_EQ(2.0, (g_val[{ 0, 0 }]));
    EXPECT_DOUBLE_EQ(-1.0, (g_val[{ 0, 1 }]));
    EXPECT_DOUBLE_EQ(-1.0, (g_val[{ 1, 0 }]));
    EXPECT_DOUBLE_EQ(1.0, (g_val[{ 1, 1 }]));

    // check symmetry of metric tensor
    EXPECT_DOUBLE_EQ((g_val[{ 0, 1 }]), (g_val[{ 1, 0 }]));
    EXPECT_DOUBLE_EQ(1.0, metric.volume_element(xi));

    // check volume element equals sqrt(det(g)): det = 2*1 - (-1)*(-1) = 1
    EXPECT_DOUBLE_EQ(std::sqrt(mito::tensor::determinant(g_val)), metric.volume_element(xi));

    // check inverse metric: g_inv should be [[1, 1], [1, 2]]
    auto g_inv = metric.g_inv(xi);
    EXPECT_DOUBLE_EQ(1.0, (g_inv[{ 0, 0 }]));
    EXPECT_DOUBLE_EQ(1.0, (g_inv[{ 0, 1 }]));
    EXPECT_DOUBLE_EQ(1.0, (g_inv[{ 1, 0 }]));
    EXPECT_DOUBLE_EQ(2.0, (g_inv[{ 1, 1 }]));

    // gradient: dphi = (1, 0), check ⟨grad, tangent_i⟩ = dphi[i]
    auto J = jacobian(xi);
    auto dphi = mito::tensor::vector_t<2>{ 1.0, 0.0 };
    auto grad = metric.gradient(dphi, xi);
    test_gradient_directions(dphi, grad, J);

    // gradient: dphi = (0, 1)
    auto dphi2 = mito::tensor::vector_t<2>{ 0.0, 1.0 };
    auto grad2 = metric.gradient(dphi2, xi);
    test_gradient_directions(dphi2, grad2, J);

    // gradient: dphi = (2, 3)
    auto dphi3 = mito::tensor::vector_t<2>{ 2.0, 3.0 };
    auto grad3 = metric.gradient(dphi3, xi);
    test_gradient_directions(dphi3, grad3, J);

    // volume form test: w should give signed area of parallelogram spanned by two vectors
    auto w = metric.w();
    auto v1 = mito::tensor::vector_t<2>{ 1.0, 0.0 };
    auto v2 = mito::tensor::vector_t<2>{ 0.0, 1.0 };
    EXPECT_DOUBLE_EQ(1.0, w(xi)(v1, v2));    // volume_element = 1

    // test antisymmetry: w(v2, v1) = -w(v1, v2)
    EXPECT_DOUBLE_EQ(-1.0, w(xi)(v2, v1));

    // test with scaled vectors
    auto v1_scaled = mito::tensor::vector_t<2>{ 2.0, 0.0 };
    auto v2_scaled = mito::tensor::vector_t<2>{ 0.0, 3.0 };
    EXPECT_DOUBLE_EQ(6.0, w(xi)(v1_scaled, v2_scaled));    // should scale as 2*3 = 6

    // test with linearly dependent vectors (area should be zero)
    auto v3 = mito::tensor::vector_t<2>{ 1.0, 0.0 };
    auto v4 = mito::tensor::vector_t<2>{ 2.0, 0.0 };
    EXPECT_DOUBLE_EQ(0.0, w(xi)(v3, v4));
}


TEST(Geometry, InducedMetricTriangle3D)
{
    // tilted triangle in 3D: (0,0,0), (1,0,1), (1,1,1), area = 0.5*sqrt(2)
    // directors: (1,0,1), (1,1,1); cross = (-1,0,1), norm = sqrt(2), area = 0.5*sqrt(2)
    auto jacobian = [](const parametric_2d_t &) {
        // columns: (1,0,1), (1,1,1)
        return mito::tensor::matrix_t<3, 2>{ 1.0, 1.0, 0.0, 1.0, 1.0, 1.0 };
    };
    auto parametrization = [](const parametric_2d_t & xi) {
        auto x0 = mito::tensor::vector_t<3>{ 0.0, 0.0, 0.0 };
        auto x1 = mito::tensor::vector_t<3>{ 1.0, 0.0, 1.0 };
        auto x2 = mito::tensor::vector_t<3>{ 1.0, 1.0, 1.0 };
        auto x = (1.0 - xi[0] - xi[1]) * x0 + xi[0] * x1 + xi[1] * x2;
        return mito::geometry::cartesian::coordinates({ x[0], x[1], x[2] });
    };

    auto metric = mito::geometry::induced_metric<2, coordinates_3d_t, parametric_2d_t>(
        jacobian, parametrization);

    auto xi = mito::geometry::cartesian::coordinates<2>({ 0.25, 0.25 });
    auto vol = metric.volume_element(xi);
    EXPECT_DOUBLE_EQ(std::sqrt(2.0), vol);

    // check volume element equals sqrt(det(g))
    auto g_val_3d = metric.g(xi);
    EXPECT_DOUBLE_EQ(std::sqrt(mito::tensor::determinant(g_val_3d)), metric.volume_element(xi));

    // check symmetry of metric tensor
    EXPECT_DOUBLE_EQ((g_val_3d[{ 0, 1 }]), (g_val_3d[{ 1, 0 }]));

    // gradient: dphi = (1, 0), check ⟨grad, tangent_i⟩ = dphi[i]
    auto J = jacobian(xi);
    auto dphi1 = mito::tensor::vector_t<2>{ 1.0, 0.0 };
    auto grad1 = metric.gradient(dphi1, xi);
    test_gradient_directions(dphi1, grad1, J);

    // gradient: dphi = (0, 1)
    auto dphi2 = mito::tensor::vector_t<2>{ 0.0, 1.0 };
    auto grad2 = metric.gradient(dphi2, xi);
    test_gradient_directions(dphi2, grad2, J);

    // gradient: dphi = (2, 3)
    auto dphi3 = mito::tensor::vector_t<2>{ 2.0, 3.0 };
    auto grad3 = metric.gradient(dphi3, xi);
    test_gradient_directions(dphi3, grad3, J);

    // partition of unity: sum of basis function gradients should be zero
    auto dphi0 = mito::tensor::vector_t<2>{ -1.0, -1.0 };
    auto grad0 = metric.gradient(dphi0, xi);
    auto sum = grad0 + grad1 + grad2;
    EXPECT_DOUBLE_EQ(0.0, sum[0]);
    EXPECT_DOUBLE_EQ(0.0, sum[1]);
    EXPECT_DOUBLE_EQ(0.0, sum[2]);

    // volume form test: w(director0, director1) gives the area element
    auto w = metric.w();
    auto director0 = mito::tensor::vector_t<2>{ 1.0, 0.0 };
    auto director1 = mito::tensor::vector_t<2>{ 0.0, 1.0 };
    EXPECT_DOUBLE_EQ(std::sqrt(2.0), w(xi)(director0, director1));

    // test antisymmetry
    EXPECT_DOUBLE_EQ(-std::sqrt(2.0), w(xi)(director1, director0));

    // test with linearly dependent vectors
    auto v_parallel = mito::tensor::vector_t<2>{ 2.0, 0.0 };
    EXPECT_DOUBLE_EQ(0.0, w(xi)(director0, v_parallel));
}


// end of file
