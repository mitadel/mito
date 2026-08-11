// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates (3D physical space)
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell (triangle embedded in 3D)
using cell_t = mito::geometry::triangle_t<3>;


TEST(Fem, BlockMassEmbeddedTriangle)
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

        // a finite element on the embedded triangle
        auto element_p1 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2 });

        // a mass matrix block
        auto density = mito::functions::one<coordinates_t>;
        constexpr int doe_mass = 2 * finite_element_t::degree;
        auto mass_block = mito::fem::blocks::value_value_block<finite_element_t, doe_mass>(density);

        // the analytical elementary mass matrix (the mass matrix is invariant under rigid
        // rotations, so it is the same as for the unit right triangle in 2D)
        auto analytical_block =
            1.0 / 24.0 * mito::tensor::matrix_t<3>{ 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0 };

        // compute the elementary contribution of the block
        auto computed_block = mass_block.compute(element_p1);

        // compute the error
        auto error = mito::tensor::norm(computed_block - analytical_block);

        // check the error is zero to machine precision
        EXPECT_NEAR(0.0, error, 1.0e-15);
    }

    // all done
    return;
}
