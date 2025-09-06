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
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the reference simplex
using reference_simplex_t = mito::geometry::reference_triangle_t;
// Gauss quadrature on triangles with degree of exactness 4
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, 4>;
// the equation map type (map associating an equation number to each node degree of freedom)
using equation_map_type = std::map<discretization_node_t, int>;


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
        // the barycentric coordinates of the quadrature point
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
        // the barycentric coordinates of the quadrature point
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

// test that the elementary stiffness matrix is computed correctly
template <class elementT>
auto
test_stiffness_matrix(
    const elementT & element, const equation_map_type & equation_map,
    const mito::tensor::matrix_t<elementT::n_nodes> & analytical_stiffness_matrix) -> void
{
    // create reporting channel
    journal::info_t channel("test_stiffness_matrix");

    // the number of quadrature points per element
    constexpr int n_quads = quadrature_rule_t::npoints;

    // the number of nodes per element
    constexpr int n_nodes = elementT::n_nodes;

    // create a {n_nodes}x{n_nodes} matrix to store the elementary stiffness matrix
    auto elementary_stiffness_matrix = mito::tensor::matrix_t<n_nodes>{};

    // loop on the quadrature points
    mito::tensor::constexpr_for_1<n_quads>([&]<int q>() {
        // the barycentric coordinates of the quadrature point
        constexpr auto xi = quadrature_rule.point(q);

        // area of the cell
        auto area = 0.5 * mito::tensor::determinant(element.jacobian()(xi));

        // precompute the common factor for integration
        auto factor = quadrature_rule.weight(q) * area;

        // populate the stiffness matrix
        mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
            // get the a-th discretization node of the element
            const auto & node_a = element.connectivity()[a];
            // evaluate the spatial gradient of the element's a-th shape function at {xi}
            auto dphi_a = element.template gradient<a>()(xi);
            // get the equation number of {node_a}
            int i = equation_map.at(node_a);
            mito::tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                // get the b-th discretization node of the element
                const auto & node_b = element.connectivity()[b];
                // evaluate the spatial gradient of the element's b-th shape function at {xi}
                auto dphi_b = element.template gradient<b>()(xi);
                // get the equation number of {node_b}
                int j = equation_map.at(node_b);
                // assemble the {node_a, node_b} contribution to the mass matrix
                elementary_stiffness_matrix[{ i, j }] += factor * dphi_a * dphi_b;
            });
        });
    });

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
    const elementT & element, const equation_map_type & equation_map,
    const mito::tensor::matrix_t<elementT::n_nodes> & analytical_mass_matrix) -> void
{
    // create reporting channel
    journal::info_t channel("test_mass_matrix");

    // the number of quadrature points per element
    constexpr int n_quads = quadrature_rule_t::npoints;

    // the number of nodes per element
    constexpr int n_nodes = elementT::n_nodes;

    // create a {n_nodes}x{n_nodes} matrix to store the elementary stiffness matrix
    auto elementary_mass_matrix = mito::tensor::matrix_t<n_nodes>{};

    // loop on the quadrature points
    mito::tensor::constexpr_for_1<n_quads>([&]<int q>() {
        // the barycentric coordinates of the quadrature point
        constexpr auto xi = quadrature_rule.point(q);

        // area of the cell
        auto factor =
            0.5 * mito::tensor::determinant(element.jacobian()(xi)) * quadrature_rule.weight(q);

        // populate the mass matrix
        mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
            // get the a-th discretization node of the element
            const auto & node_a = element.connectivity()[a];
            // evaluate element's a-th shape function at {xi}
            auto phi_a = element.template shape<a>()(xi);
            // get the equation number of {node_a}
            int i = equation_map.at(node_a);
            mito::tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                // get the b-th discretization node of the element
                const auto & node_b = element.connectivity()[b];
                // evaluate element's b-th shape function at {xi}
                auto phi_b = element.template shape<b>()(xi);
                // get the equation number of {node_b}
                int j = equation_map.at(node_b);
                // assemble the {node_a, node_b} contribution to the mass matrix
                elementary_mass_matrix[{ i, j }] += factor * phi_a * phi_b;
            });
        });
    });

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

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::triangle<2>({ node_0, node_1, node_2 });

    {
        // first order isoparametric triangle
        using element_p1_t = mito::fem::isoparametric_simplex<1, cell_t>::type;

        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();

        // a finite element
        auto element_p1 = element_p1_t(
            geometric_simplex, coord_system,
            { discretization_node_0, discretization_node_1, discretization_node_2 });

        // check that first order shape functions are a partition of unity
        test_partition_of_unity(element_p1);

        // check that the gradients of first order shape functions sum to 0.0
        test_gradient_consistency(element_p1);

        // the map between the discretization nodes and the equation numbers
        equation_map_type equation_map;

        // populate the equation map
        equation_map[discretization_node_0] = 0;
        equation_map[discretization_node_1] = 1;
        equation_map[discretization_node_2] = 2;

        // the analytical elementary stiffness matrix
        auto analytical_stiffness_matrix =
            1.0 / 2.0
            * mito::tensor::matrix_t<3>{ 2.0, -1.0, -1.0, -1.0, 1.0, 0.0, -1.0, 0.0, 1.0 };

        // check that the elementary stiffness matrix is computed correctly
        test_stiffness_matrix(element_p1, equation_map, analytical_stiffness_matrix);

        // the analytical elementary mass matrix
        auto analytical_mass_matrix =
            1.0 / 24.0 * mito::tensor::matrix_t<3>{ 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0 };

        test_mass_matrix(element_p1, equation_map, analytical_mass_matrix);
    }

    {
        // second order isoparametric triangle
        using element_p2_t = mito::fem::isoparametric_simplex<2, cell_t>::type;

        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();
        auto discretization_node_3 = discretization_node_t();
        auto discretization_node_4 = discretization_node_t();
        auto discretization_node_5 = discretization_node_t();

        // a finite element
        auto element_p2 = element_p2_t(
            geometric_simplex, coord_system,
            { discretization_node_0, discretization_node_1, discretization_node_2,
              discretization_node_3, discretization_node_4, discretization_node_5 });

        // check that second order shape functions are a partition of unity
        test_partition_of_unity(element_p2);

        // check that the gradients of second order shape functions sum to 0.0
        test_gradient_consistency(element_p2);

        // the map between the discretization nodes and the equation numbers
        equation_map_type equation_map;

        // populate the equation map
        equation_map[discretization_node_0] = 0;
        equation_map[discretization_node_1] = 1;
        equation_map[discretization_node_2] = 2;
        equation_map[discretization_node_3] = 3;
        equation_map[discretization_node_4] = 4;
        equation_map[discretization_node_5] = 5;

        // the analytical elementary stiffness matrix
        auto analytical_stiffness_matrix = mito::tensor::matrix_t<6>{
            1.0,        1.0 / 6.0,  1.0 / 6.0,  -2.0 / 3.0, 0.0,        -2.0 / 3.0,
            1.0 / 6.0,  1.0 / 2.0,  0.0,        -2.0 / 3.0, 0.0,        0.0,
            1.0 / 6.0,  0.0,        1.0 / 2.0,  0.0,        0.0,        -2.0 / 3.0,
            -2.0 / 3.0, -2.0 / 3.0, 0.0,        8.0 / 3.0,  -4.0 / 3.0, 0.0,
            0.0,        0.0,        0.0,        -4.0 / 3.0, 8.0 / 3.0,  -4.0 / 3.0,
            -2.0 / 3.0, 0.0,        -2.0 / 3.0, 0.0,        -4.0 / 3.0, 8.0 / 3.0
        };

        // check that the elementary stiffness matrix is computed correctly
        test_stiffness_matrix(element_p2, equation_map, analytical_stiffness_matrix);

        // the analytical elementary mass matrix
        auto analytical_mass_matrix = mito::tensor::matrix_t<6>{
            1.0 / 60.0,   -1.0 / 360.0, -1.0 / 360.0, 0.0,         -1.0 / 90.0, 0.0,
            -1.0 / 360.0, 1.0 / 60.0,   -1.0 / 360.0, 0.0,         0.0,         -1.0 / 90.0,
            -1.0 / 360.0, -1.0 / 360.0, 1.0 / 60.0,   -1.0 / 90.0, 0.0,         0.0,
            0.0,          0.0,          -1.0 / 90.0,  4.0 / 45.0,  2.0 / 45.0,  2.0 / 45.0,
            -1.0 / 90.0,  0.0,          0.0,          2.0 / 45.0,  4.0 / 45.0,  2.0 / 45.0,
            0.0,          -1.0 / 90.0,  0.0,          2.0 / 45.0,  2.0 / 45.0,  4.0 / 45.0
        };

        test_mass_matrix(element_p2, equation_map, analytical_mass_matrix);
    }

    // all done
    return;
}