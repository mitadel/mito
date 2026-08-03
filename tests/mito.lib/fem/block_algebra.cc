// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;


TEST(Fem, BlockSum)
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

    // build the discretization nodes
    auto discretization_node_0 = discretization_node_t();
    auto discretization_node_1 = discretization_node_t();
    auto discretization_node_2 = discretization_node_t();

    // the degree of the finite element
    constexpr int degree = 1;
    // assemble the finite element type
    using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

    // a finite element
    auto element_p1 = mito::fem::finite_element<finite_element_t>(
        element, { discretization_node_0, discretization_node_1, discretization_node_2 });

    // a mass matrix block
    constexpr auto mass_block = mito::fem::blocks::mass_block<finite_element_t>();

    // a grad-grad matric block
    constexpr auto grad_grad_block = mito::fem::blocks::grad_grad_block<finite_element_t>();

    // add them up
    constexpr auto sum_block = mass_block + grad_grad_block;

    // the analytical elementary mass matrix
    auto analytical_block = 1.0 / 24.0 * mito::tensor::matrix_t<3>{ 26.0, -11.0, -11.0, -11.0, 14.0,
                                                                    1.0,  -11.0, 1.0,   14.0 };

    // compute the elementary contribution of the block
    auto computed_block = sum_block.compute(element_p1);

    // compute the error
    auto error = mito::tensor::norm(computed_block - analytical_block);

    // check the error is reasonable
    EXPECT_DOUBLE_EQ(0.0, error);

    // all done
    return;
}

TEST(Fem, BlockProduct)
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

    // build the discretization nodes
    auto discretization_node_0 = discretization_node_t();
    auto discretization_node_1 = discretization_node_t();
    auto discretization_node_2 = discretization_node_t();

    // the degree of the finite element
    constexpr int degree = 1;
    // assemble the finite element type
    using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

    // a finite element
    auto element_p1 = mito::fem::finite_element<finite_element_t>(
        element, { discretization_node_0, discretization_node_1, discretization_node_2 });

    // a mass matrix block
    constexpr auto mass_block = mito::fem::blocks::mass_block<finite_element_t>();

    // add them up
    constexpr auto product_block = 2.0 * mass_block;

    // the analytical elementary mass matrix
    auto analytical_block =
        1.0 / 12.0 * mito::tensor::matrix_t<3>{ 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0 };

    // compute the elementary contribution of the block
    auto computed_block = product_block.compute(element_p1);

    // compute the error
    auto error = mito::tensor::norm(computed_block - analytical_block);

    // check the error is reasonable
    EXPECT_NEAR(0.0, error, 1e-16);

    // all done
    return;
}

TEST(Fem, BlockProductSum)
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

    // build the discretization nodes
    auto discretization_node_0 = discretization_node_t();
    auto discretization_node_1 = discretization_node_t();
    auto discretization_node_2 = discretization_node_t();

    // the degree of the finite element
    constexpr int degree = 1;
    // assemble the finite element type
    using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

    // a finite element
    auto element_p1 = mito::fem::finite_element<finite_element_t>(
        element, { discretization_node_0, discretization_node_1, discretization_node_2 });

    // a mass matrix block
    constexpr auto mass_block = mito::fem::blocks::mass_block<finite_element_t>();

    // a grad-grad matric block
    constexpr auto grad_grad_block = mito::fem::blocks::grad_grad_block<finite_element_t>();

    // add them up
    constexpr auto sum_block = 2.0 * mass_block + (-1.0) * grad_grad_block;

    // the analytical elementary mass matrix
    auto analytical_block =
        mito::tensor::matrix_t<3>{ -10.0 / 12.0, 7.0 / 12.0, 7.0 / 12.0, 7.0 / 12.0, -4.0 / 12.0,
                                   1.0 / 12.0,   7.0 / 12.0, 1.0 / 12.0, -4.0 / 12.0 };

    // compute the elementary contribution of the block
    auto computed_block = sum_block.compute(element_p1);

    // compute the error
    auto error = mito::tensor::norm(computed_block - analytical_block);

    // check the error is reasonable
    EXPECT_NEAR(0.0, error, 1e-15);

    // all done
    return;
}

// end of file
