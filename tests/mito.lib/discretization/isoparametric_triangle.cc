// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discretization::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// Gauss quadrature on triangles with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, cell_t::simplex_type, 3>;


// instantiate the quadrature rule
constexpr auto quadrature_rule = quadrature_rule_t();


// test that all shape functions sum to 1.0 at any quadrature point
auto
test_partition_of_unity(const auto & element)
{
    // loop on the quadrature points
    for (int q = 0; q < quadrature_rule_t::npoints; ++q) {

        // the barycentric coordinates of the quadrature point
        auto xi = quadrature_rule.point(q);

        // evaluate the element shape functions at {xi}
        auto phi = element.shape(xi);

        // the sum of the shape functions
        auto sum = 0.0;

        // populate the linear system of equations
        for (const auto & [_, phi_a] : phi) {
            sum += phi_a;
        }

        // check the sum of the shape functions
        EXPECT_DOUBLE_EQ(1.0, sum);
    }

    // all done
    return;
}

// test that the gradients of all shape functions sum to 0.0 at any quadrature point
auto
test_gradient_consistency(const auto & element)
{
    // loop on the quadrature points
    for (int q = 0; q < quadrature_rule_t::npoints; ++q) {

        // the barycentric coordinates of the quadrature point
        auto xi = quadrature_rule.point(q);

        // evaluate the element shape functions gradients at {xi}
        auto dphi = element.gradient(xi);

        // the sum of the shape functions
        auto sum = mito::tensor::vector_t<2>{ 0.0, 0.0 };

        // populate the linear system of equations
        for (const auto & [_, dphi_a] : dphi) {
            sum += dphi_a;
        }

        // check the sum of the shape functions gradients
        EXPECT_NEAR(0.0, sum[0], 3.0e-16);
        EXPECT_NEAR(0.0, sum[1], 3.0e-16);
    }

    // all done
    return;
}

// test that the elementary stiffness matrix is computed correctly
template <class elementT>
auto
test_stiffness_matrix(
    const elementT & element,
    const mito::tensor::matrix_t<elementT::n_nodes> & analytical_stiffness_matrix) -> void
{
    // create reporting channel
    journal::info_t channel("test_stiffness_matrix");

    // create a {n_nodes}x{n_nodes} matrix to store the elementary stiffness matrix
    auto elementary_stiffness_matrix = mito::tensor::matrix_t<elementT::n_nodes>();

    // loop on the quadrature points
    for (int q = 0; q < quadrature_rule_t::npoints; ++q) {

        // the barycentric coordinates of the quadrature point
        auto xi = quadrature_rule.point(q);

        // area of the cell
        auto area = 0.5 * mito::tensor::determinant(element.jacobian(xi));

        // evaluate the element shape functions gradients at {xi}
        auto dphi = element.gradient(xi);

        // precompute the common factor for integration
        auto factor = quadrature_rule.weight(q) * area;

        // populate the mass matrix
        int i = 0;
        for (const auto & [node_a, dphi_a] : dphi) {
            int j = 0;
            for (const auto & [node_b, dphi_b] : dphi) {
                // assemble the {node_a, node_b} contribution to the mass matrix
                elementary_stiffness_matrix[{ i, j }] += factor * dphi_a * dphi_b;
                // increment the column index
                ++j;
            }
            // increment the row index
            ++i;
        }
    }

    // compute the error
    auto error = mito::tensor::norm(elementary_stiffness_matrix - analytical_stiffness_matrix);

    // check the error is reasonable
    EXPECT_NEAR(0.0, error, 1.e-14);

    // all done
    return;
}

// test that the elementary mass matrix is computed correctly
template <class elementT>
auto
test_mass_matrix(
    const elementT & element,
    const mito::tensor::matrix_t<elementT::n_nodes> & analytical_mass_matrix) -> void
{
    // create reporting channel
    journal::info_t channel("test_mass_matrix");

    // create a {n_nodes}x{n_nodes} matrix to store the elementary stiffness matrix
    auto elementary_mass_matrix = mito::tensor::matrix_t<elementT::n_nodes>();

    // loop on the quadrature points
    for (int q = 0; q < quadrature_rule_t::npoints; ++q) {

        // the barycentric coordinates of the quadrature point
        auto xi = quadrature_rule.point(q);

        // evaluate the element shape functions at {xi}
        auto phi = element.shape(xi);

        // area of the cell
        auto factor =
            0.5 * mito::tensor::determinant(element.jacobian(xi)) * quadrature_rule.weight(q);

        // populate the mass matrix
        int i = 0;
        for (const auto & [_, phi_a] : phi) {
            int j = 0;
            for (const auto & [_, phi_b] : phi) {
                // assemble the {node_a, node_b} contribution to the mass matrix
                elementary_mass_matrix[{ i, j }] += factor * phi_a * phi_b;
                // increment the column index
                ++j;
            }
            // increment the row index
            ++i;
        }
    }

    // compute the error
    auto error = mito::tensor::norm(elementary_mass_matrix - analytical_mass_matrix);

    // check the error is reasonable
    EXPECT_NEAR(0.0, error, 1.e-14);

    // all done
    return;
}

TEST(Fem, IsoparametricTriangle)
{
    // the coordinate system
    auto coord_system = coord_system_t();

    // the origin of the coordinate system
    auto origin = typename coord_system_t::coordinates_type{};

    // build nodes
    auto coord_0 = coordinates_t{ 0.0, 0.0 };
    auto node_0 = mito::geometry::node(coord_system, coord_0);
    auto coord_1 = coordinates_t{ 1.0, 0.0 };
    auto node_1 = mito::geometry::node(coord_system, coord_1);
    auto coord_2 = coordinates_t{ 0.0, 1.0 };
    auto node_2 = mito::geometry::node(coord_system, coord_2);

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::triangle<2>({ node_0, node_1, node_2 });

    {
        // first order isoparametric triangle
        using element_p1_t =
            typename mito::discretization::isoparametric_simplex<1, cell_t, coord_system_t>::type;

        // a finite element
        auto element_p1 = element_p1_t(
            geometric_simplex,
            { discretization_node_t(), discretization_node_t(), discretization_node_t() },
            coord_0 - origin, coord_1 - origin, coord_2 - origin);

        // check that first order shape functions are a partition of unity
        test_partition_of_unity(element_p1);

        // check that the gradients of first order shape functions sum to 0.0
        test_gradient_consistency(element_p1);

        // the analytical elementary stiffness matrix
        auto analytical_stiffness_matrix =
            1.0 / 2.0
            * mito::tensor::matrix_t<3>({ 2.0, -1.0, -1.0, -1.0, 1.0, 0.0, -1.0, 0.0, 1.0 });

        // check that the elementary stiffness matrix is computed correctly
        test_stiffness_matrix(element_p1, analytical_stiffness_matrix);

        // the analytical elementary mass matrix
        auto analytical_mass_matrix =
            1.0 / 24.0 * mito::tensor::matrix_t<3>({ 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0 });

        test_mass_matrix(element_p1, analytical_mass_matrix);
    }

    {
        // second order isoparametric triangle
        using element_p2_t =
            typename mito::discretization::isoparametric_simplex<2, cell_t, coord_system_t>::type;

        // a finite element
        auto element_p2 = element_p2_t(
            geometric_simplex,
            { discretization_node_t(), discretization_node_t(), discretization_node_t(),
              discretization_node_t(), discretization_node_t(), discretization_node_t() },
            coord_0 - origin, coord_1 - origin, coord_2 - origin);

        // check that second order shape functions are a partition of unity
        test_partition_of_unity(element_p2);

        // check that the gradients of second order shape functions sum to 0.0
        test_gradient_consistency(element_p2);
    }

    // all done
    return;
}