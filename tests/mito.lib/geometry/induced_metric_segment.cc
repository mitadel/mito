// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <cmath>


using coordinates_1d_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
using coordinates_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coordinates_3d_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


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

// helper function to compute gradient magnitude
template <int DIM>
double
gradient_magnitude(const mito::tensor::vector_t<DIM> & grad)
{
    double result = 0.0;
    for (int i = 0; i < DIM; ++i) {
        result += grad[i] * grad[i];
    }
    return std::sqrt(result);
}


TEST(Geometry, InducedMetricSegment1D)
{
    // unit segment in 1D: x(xi) = xi, J = 1, g_induced = 1,
    // volume_element = 1 (equals length of segment)
    auto jacobian = [](const coordinates_1d_t &) {
        return mito::tensor::matrix_t<1, 1>{ 1.0 };
    };
    auto parametrization = [](const coordinates_1d_t & xi) {
        return mito::geometry::cartesian::coordinates({ xi[0] });
    };

    auto metric = mito::geometry::induced_metric<1, coordinates_1d_t, coordinates_1d_t>(
        jacobian, parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.5 });
    auto g_val = metric.g(xi);
    EXPECT_DOUBLE_EQ(1.0, mito::tensor::trace(g_val));    // check g
    EXPECT_DOUBLE_EQ(1.0, metric.volume_element(xi));     // check segment length

    // check inverse metric
    auto g_inv = metric.g_inv(xi);
    EXPECT_DOUBLE_EQ(1.0, mito::tensor::trace(g_inv));

    // gradient: dphi/dxi = 1, check ⟨grad, tangent⟩ = dphi/dxi
    auto grad = metric.gradient(1.0, xi);
    auto J = jacobian(xi);
    EXPECT_DOUBLE_EQ(1.0, directional_derivative(grad, J, 0));

    // gradient magnitude: |grad| = 1/|tangent| = 1
    EXPECT_DOUBLE_EQ(1.0, gradient_magnitude(grad));

    // nonlinear test: phi(xi) = xi^2, dphi/dxi = 2*xi
    auto grad_nonlinear = metric.gradient(2.0 * xi[0], xi);
    EXPECT_DOUBLE_EQ(2.0 * xi[0], directional_derivative(grad_nonlinear, J, 0));

    // trigonometric test: phi(xi) = sin(xi), dphi/dxi = cos(xi)
    auto grad_trig = metric.gradient(std::cos(xi[0]), xi);
    EXPECT_DOUBLE_EQ(std::cos(xi[0]), directional_derivative(grad_trig, J, 0));
}


TEST(Geometry, InducedMetricSegment2D)
{
    // diagonal segment from (0,0) to (3,4): x(xi) = (3*xi, 4*xi), J = [3; 4], g_induced = 25,
    // volume element = 5 (equals length of segment)
    auto jacobian = [](const coordinates_1d_t &) {
        return mito::tensor::matrix_t<2, 1>{ 3.0, 4.0 };
    };
    auto parametrization = [](const coordinates_1d_t & xi) {
        return mito::geometry::cartesian::coordinates({ 3.0 * xi[0], 4.0 * xi[0] });
    };

    auto metric = mito::geometry::induced_metric<1, coordinates_2d_t, coordinates_1d_t>(
        jacobian, parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.5 });
    auto g_val = metric.g(xi);
    EXPECT_DOUBLE_EQ(25.0, mito::tensor::trace(g_val));    // check g
    EXPECT_DOUBLE_EQ(5.0, metric.volume_element(xi));      // check segment length

    // check inverse metric: g * g_inv should be identity
    auto g_inv = metric.g_inv(xi);
    EXPECT_DOUBLE_EQ(1.0 / 25.0, mito::tensor::trace(g_inv));

    // gradient: dphi/dxi = 1, check ⟨grad, tangent⟩ = dphi/dxi
    auto grad = metric.gradient(1.0, xi);
    auto J = jacobian(xi);
    EXPECT_DOUBLE_EQ(1.0, directional_derivative(grad, J, 0));

    // gradient magnitude: |grad| = 1/|tangent| = 1/5
    EXPECT_DOUBLE_EQ(0.2, gradient_magnitude(grad));

    // nonlinear test: phi(xi) = xi^2, dphi/dxi = 2*xi
    auto grad_nonlinear = metric.gradient(2.0 * xi[0], xi);
    EXPECT_DOUBLE_EQ(2.0 * xi[0], directional_derivative(grad_nonlinear, J, 0));

    // trigonometric test: phi(xi) = sin(xi), dphi/dxi = cos(xi)
    auto grad_trig = metric.gradient(std::cos(xi[0]), xi);
    EXPECT_DOUBLE_EQ(std::cos(xi[0]), directional_derivative(grad_trig, J, 0));

    // volume form test: w(tangent) = volume_element * tangent
    auto w = metric.w();
    auto tangent = mito::tensor::vector_t<1>{ 1.0 };
    EXPECT_DOUBLE_EQ(5.0, w(xi)(tangent));    // volume_element = 5

    // test scaling property
    auto tangent2 = mito::tensor::vector_t<1>{ 2.0 };
    EXPECT_DOUBLE_EQ(10.0, w(xi)(tangent2));

    // test with negative tangent
    auto tangent_neg = mito::tensor::vector_t<1>{ -1.0 };
    EXPECT_DOUBLE_EQ(-5.0, w(xi)(tangent_neg));
}


TEST(Geometry, InducedMetricSegment3D)
{
    // segment in 3D from (0,0,0) to (1,1,1): J = [1;1;1], g_induced = 3,
    // volume_element = sqrt(3) (equals length of segment)
    auto jacobian = [](const coordinates_1d_t &) {
        return mito::tensor::matrix_t<3, 1>{ 1.0, 1.0, 1.0 };
    };
    auto parametrization = [](const coordinates_1d_t & xi) {
        return mito::geometry::cartesian::coordinates({ xi[0], xi[0], xi[0] });
    };

    auto metric = mito::geometry::induced_metric<1, coordinates_3d_t, coordinates_1d_t>(
        jacobian, parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.25 });
    auto g_val = metric.g(xi);
    EXPECT_DOUBLE_EQ(3.0, mito::tensor::trace(g_val));              // check g
    EXPECT_DOUBLE_EQ(std::sqrt(3.0), metric.volume_element(xi));    // check segment length

    // check inverse metric
    auto g_inv = metric.g_inv(xi);
    EXPECT_DOUBLE_EQ(1.0 / 3.0, mito::tensor::trace(g_inv));

    // gradient: dphi/dxi = 1, check ⟨grad, tangent⟩ = dphi/dxi
    auto grad = metric.gradient(1.0, xi);
    auto J = jacobian(xi);
    EXPECT_DOUBLE_EQ(1.0, directional_derivative(grad, J, 0));

    // gradient magnitude: |grad| = 1/|tangent| = 1/sqrt(3)
    EXPECT_DOUBLE_EQ(1.0 / std::sqrt(3.0), gradient_magnitude(grad));

    // nonlinear test: phi(xi) = xi^2, dphi/dxi = 2*xi
    auto grad_nonlinear = metric.gradient(2.0 * xi[0], xi);
    EXPECT_DOUBLE_EQ(2.0 * xi[0], directional_derivative(grad_nonlinear, J, 0));

    // trigonometric test: phi(xi) = sin(xi), dphi/dxi = cos(xi)
    auto grad_trig = metric.gradient(std::cos(xi[0]), xi);
    EXPECT_DOUBLE_EQ(std::cos(xi[0]), directional_derivative(grad_trig, J, 0));

    // volume form test: w(tangent) = volume_element * tangent
    auto w = metric.w();
    auto tangent = mito::tensor::vector_t<1>{ 1.0 };
    EXPECT_DOUBLE_EQ(std::sqrt(3.0), w(xi)(tangent));    // volume_element = sqrt(3)

    // test scaling property
    auto tangent2 = mito::tensor::vector_t<1>{ 0.5 };
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(3.0), w(xi)(tangent2));
}


// end of file
