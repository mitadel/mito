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
    auto coord_2 = coordinates_t{ 1.0, 1.0 };
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