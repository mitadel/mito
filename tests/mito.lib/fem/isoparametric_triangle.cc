// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the reference simplex
using reference_simplex_t = cell_t::reference_simplex_type;
// Gauss quadrature on triangles with degree of exactness 4
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, 4>;


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

        // compute the sum of the shape functions at {xi} for all nodes
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


TEST(Fem, IsoparametricTriangle)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // an atlas under the coordinate system
    auto atlas = mito::manifolds::atlas<cell_t>(coord_system);

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // make a geometric simplex
    auto triangle = mito::geometry::triangle(node_0, node_1, node_2);

    // make a manifold element from the triangle
    auto element = mito::manifolds::parametrized_element(
        triangle, atlas.parametrization(triangle), metric_space_t::w);

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();

        // the degree of the finite element
        constexpr int degree = 1;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // first order isoparametric finite element
        auto element_p1 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2 });

        // check that first order shape functions are a partition of unity
        test_partition_of_unity(element_p1);

        // check that the gradients of first order shape functions sum to 0.0
        test_gradient_consistency(element_p1);
    }

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();
        auto discretization_node_3 = discretization_node_t();
        auto discretization_node_4 = discretization_node_t();
        auto discretization_node_5 = discretization_node_t();

        // the degree of the finite element
        constexpr int degree = 2;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // second order isoparametric finite element
        auto element_p2 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2,
                       discretization_node_3, discretization_node_4, discretization_node_5 });

        // check that second order shape functions are a partition of unity
        test_partition_of_unity(element_p2);

        // check that the gradients of second order shape functions sum to 0.0
        test_gradient_consistency(element_p2);
    }

    // all done
    return;
}