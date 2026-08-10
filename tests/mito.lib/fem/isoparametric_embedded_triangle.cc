// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates (3D physical space)
using coordinates_t = mito::geometry::cartesian::coordinates_t<3>;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell (triangle embedded in 3D)
using cell_t = mito::geometry::triangle_t<3>;
// the reference simplex
using reference_simplex_t = cell_t::reference_simplex_type;
// Gauss quadrature on triangles with degree of exactness 2
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

        // check that the sum of the shape functions gradients is the zero vector
        EXPECT_NEAR(0.0, mito::tensor::norm(sum), 3.0e-16);
    });

    // all done
    return;
}

TEST(Fem, IsoparametricEmbeddedTriangle)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // an atlas under the coordinate system
    auto atlas = mito::manifolds::atlas<cell_t>(coord_system);

    // build nodes for a unit right triangle rigidly rotated out of the xy-plane
    // (the legs {t_1} = (0.6, 0.0, 0.8) and {t_2} = (0.0, 1.0, 0.0) are unit and orthogonal)
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 0.6, 0.0, 0.8 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });

    // make a geometric simplex
    auto triangle = mito::geometry::triangle(node_0, node_1, node_2);

    // the normal field to the triangle: {n} = {t_1} x {t_2} normalized,
    // oriented such that w(n, t_1, t_2) > 0
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<3>{ -0.8, 0.0, 0.6 });

    // strip the namespace from the placeholder for forms contractions
    using mito::tensor::_;

    // the ambient metric volume form
    constexpr auto w = metric_space_t::w;

    // the restriction of the metric volume form to the triangle
    auto wS = mito::functions::function(
        [w, normal_field](const coordinates_t & x) { return w(x)(normal_field(x), _, _); });

    // make a manifold element from the triangle
    auto element =
        mito::manifolds::parametrized_element(triangle, atlas.parametrization(triangle), wS);

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();

        // the degree of the finite element
        constexpr int degree = 1;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // first order isoparametric finite element on the embedded triangle
        auto element_p1 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2 });

        // check that first order shape functions are a partition of unity
        test_partition_of_unity(element_p1);

        // check that the gradients of first order shape functions sum to 0.0
        test_gradient_consistency(element_p1);

        // check the gradient values at the barycenter
        {
            auto xi = reference_simplex_t::parametric_coordinates_type{ 1.0 / 3.0, 1.0 / 3.0 };
            auto grad_0 = element_p1.gradient<0>()(xi);
            auto grad_1 = element_p1.gradient<1>()(xi);
            auto grad_2 = element_p1.gradient<2>()(xi);

            // the tangent vectors (the triangle legs) and the normal vector
            constexpr auto t_1 = mito::tensor::vector_t<3>{ 0.6, 0.0, 0.8 };
            constexpr auto t_2 = mito::tensor::vector_t<3>{ 0.0, 1.0, 0.0 };
            constexpr auto n = mito::tensor::vector_t<3>{ -0.8, 0.0, 0.6 };

            // the gradients lie in the plane of the triangle
            EXPECT_NEAR(0.0, grad_0 * n, 1.0e-15);
            EXPECT_NEAR(0.0, grad_1 * n, 1.0e-15);
            EXPECT_NEAR(0.0, grad_2 * n, 1.0e-15);

            // the directional derivatives along the legs recover the parametric derivatives
            EXPECT_NEAR(-1.0, grad_0 * t_1, 1.0e-15);
            EXPECT_NEAR(-1.0, grad_0 * t_2, 1.0e-15);
            EXPECT_NEAR(1.0, grad_1 * t_1, 1.0e-15);
            EXPECT_NEAR(0.0, grad_1 * t_2, 1.0e-15);
            EXPECT_NEAR(0.0, grad_2 * t_1, 1.0e-15);
            EXPECT_NEAR(1.0, grad_2 * t_2, 1.0e-15);
        }
    }

    // all done
    return;
}
