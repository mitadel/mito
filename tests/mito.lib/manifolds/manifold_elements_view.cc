// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>
#include <mito/io.h>


// the type of coordinates
using coordinates_t = mito::geometry::cartesian::coordinates_t<2>;

// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;


TEST(Manifolds, ElementView)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert triangles in mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // create a manifold on {mesh} with the appropriate metric volume form
    auto manifold = mito::manifolds::manifold(mesh, coord_system, metric_space_t::w);

    // compute the volume of the manifold
    auto area = 0.0;
    for (const auto & e : manifold.elements()) {
        // get the parametrization of this element
        auto phi = e.parametrization();
        // compute the derivative of the cell parametrization
        const auto J = mito::functions::derivative(phi);
        // get the metric volume form of this element
        auto w = e.metric_volume_form();
        // get the director edges of this cell and the point where they stem from
        auto [point, directors] = mito::geometry::directors(e.cell(), coord_system);
        // compute the area of the cell
        area += 1.0 / 2 * w(point)(directors);
    }

    // check that the result of the calculation is correct
    EXPECT_DOUBLE_EQ(area, 1.0);
}


// end of file
