// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::segment_t<1>;
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
        EXPECT_NEAR(0.0, sum, 3.0e-16);
    });

    // all done
    return;
}


TEST(Fem, IsoparametricSegment)
{
    // the coordinate system
    auto coord_system = coord_system_t();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::segment<1>({ node_0, node_1 });

    {
        // first order isoparametric segment
        using element_p1_t = mito::fem::isoparametric_simplex_t<1, cell_t>;

        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();

        // a finite element
        auto element_p1 = element_p1_t(
            geometric_simplex, coord_system, { discretization_node_0, discretization_node_1 });

        // check that first order shape functions are a partition of unity
        test_partition_of_unity(element_p1);

        // check that the gradients of first order shape functions sum to 0.0
        test_gradient_consistency(element_p1);
    }

    // all done
    return;
}
