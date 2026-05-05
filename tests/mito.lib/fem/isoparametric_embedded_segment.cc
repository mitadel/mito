// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates (2D physical space for embedded segments)
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell (segment embedded in 2D)
using cell_t = mito::geometry::segment_t<2>;
// the reference simplex
using reference_simplex_t = mito::geometry::reference_segment_t;
// Gauss quadrature on segments with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, 2>;


// instantiate the quadrature rule
constexpr auto quadrature_rule = quadrature_rule_t();


// test that all shape functions sum to 1.0 at any quadrature point
auto
test_partition_of_unity(const auto & element)
{
    // the number of quadrature points per element
    constexpr int n_quads = quadrature_rule_t::npoints;

    // the number of nodes per element
    constexpr int n_nodes = mito::utilities::base_type<decltype(element)>::n_nodes;

    // loop on the quadrature points
    mito::tensor::constexpr_for_1<n_quads>([&]<int q>() {
        // the parametric coordinates of the quadrature point
        constexpr auto xi = quadrature_rule.point(q);

        // compute the sum of the shape functions at {xi} for all nodes
        constexpr auto sum =
            ([]<int... a>(
                 const auto & element, const auto & xi, mito::tensor::integer_sequence<a...>) {
                return ((element.template shape<a>()(xi)) + ...);
            })(element, xi, mito::tensor::make_integer_sequence<n_nodes>{});

        // check the sum of the shape functions
        static_assert(1.0 == sum);
    });

    // all done
    return;
}

// test that the gradients of all shape functions sum to 0.0 at any quadrature point
auto
test_gradient_consistency(const auto & element)
{
    // the number of quadrature points per element
    constexpr int n_quads = quadrature_rule_t::npoints;

    // the number of nodes per element
    constexpr int n_nodes = mito::utilities::base_type<decltype(element)>::n_nodes;

    // loop on the quadrature points
    mito::tensor::constexpr_for_1<n_quads>([&]<int q>() {
        // the parametric coordinates of the quadrature point
        constexpr auto xi = quadrature_rule.point(q);

        // compute the sum of the shape functions gradients at {xi} for all nodes
        auto sum =
            ([]<int... a>(
                 const auto & element, const auto & xi, mito::tensor::integer_sequence<a...>) {
                return ((element.template gradient<a>()(xi)) + ...);
            })(element, xi, mito::tensor::make_integer_sequence<n_nodes>{});

        // check the sum of the shape functions gradients
        EXPECT_NEAR(0.0, sum[0], 3.0e-16);
        EXPECT_NEAR(0.0, sum[1], 3.0e-16);
    });

    // all done
    return;
}

// test that the arc length computed via the volume element matches the expected value
auto
test_arc_length(const auto & element, double expected_length)
{
    // the element type
    using element_t = mito::utilities::base_type<decltype(element)>;

    // the number of quadrature points per element
    constexpr int n_quads = quadrature_rule_t::npoints;

    // compute the arc length by integrating 1 over the segment using the volume element
    double arc_length = 0.0;
    mito::tensor::constexpr_for_1<n_quads>([&]<int q>() {
        constexpr auto xi = quadrature_rule.point(q);
        constexpr auto w = element_t::canonical_element_type::area * quadrature_rule.weight(q);
        arc_length += w * element.volume_element()(xi);
    });

    // check the arc length
    EXPECT_NEAR(expected_length, arc_length, 1.0e-15);

    // all done
    return;
}

TEST(Fem, IsoparametricEmbeddedSegment)
{
    // the coordinate system
    auto coord_system = coord_system_t();

    // build nodes for a diagonal segment from (0,0) to (3,4) - length 5
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 3.0, 4.0 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::segment<2>({ node_0, node_1 });

    // create a mesh with a single segment
    auto mesh = mito::mesh::mesh<cell_t>();
    mesh.insert({ node_0, node_1 });

    // create normal field for the submanifold (perpendicular to segment direction)
    // segment direction is (3,4)/5 = (0.6, 0.8), so normal is (0.8, -0.6)
    // (rotated 90° clockwise to get positive orientation with w(normal, tangent) > 0)
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.8, -0.6 });

    // create the submanifold
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    // first order isoparametric embedded segment
    using element_p1_t = mito::fem::isoparametric_simplex_t<1, decltype(manifold)>;

    // build the discretization nodes
    auto discretization_node_0 = discretization_node_t();
    auto discretization_node_1 = discretization_node_t();

    // a finite element
    auto element_p1 = element_p1_t(
        geometric_simplex, coord_system, { discretization_node_0, discretization_node_1 },
        manifold.volume_form());

    // check that first order shape functions are a partition of unity
    test_partition_of_unity(element_p1);

    // check that the gradients of first order shape functions sum to 0.0
    test_gradient_consistency(element_p1);

    // check that the arc length is sqrt(3^2 + 4^2) = 5
    test_arc_length(element_p1, 5.0);

    // check the gradient values at the midpoint for the (0,0) to (3,4) segment
    {
        auto xi = mito::geometry::reference_segment_t::parametric_coordinates_type{ 0.5 };
        auto grad_0 = element_p1.gradient<0>()(xi);
        auto grad_1 = element_p1.gradient<1>()(xi);

        // for linear shape functions, the gradients should be constant and opposite
        // the gradient should be in the direction of the segment: (3/5, 4/5) / 5 = (3/25, 4/25)
        // phi_0 decreases from 1 to 0, so grad_0 = (-3/25, -4/25)
        // phi_1 increases from 0 to 1, so grad_1 = (3/25, 4/25)
        EXPECT_NEAR(-3.0 / 25.0, grad_0[0], 1.0e-15);
        EXPECT_NEAR(-4.0 / 25.0, grad_0[1], 1.0e-15);
        EXPECT_NEAR(3.0 / 25.0, grad_1[0], 1.0e-15);
        EXPECT_NEAR(4.0 / 25.0, grad_1[1], 1.0e-15);
    }

    // all done
    return;
}
