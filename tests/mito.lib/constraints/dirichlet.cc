// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/constraints.h>
#include <mito/mesh.h>
#include <mito/io.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;


// the function extracting the x component of a 2D vector
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the function extracting the y component of a 2D vector
constexpr auto y = mito::functions::component<coordinates_t, 1>;


TEST(Constraints, Dirichlet)
{
    // make a channel
    journal::info_t channel("tests.dirichlet_constraints");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a unit disk in 2D
    std::ifstream fileStream("disk_cartesian.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, coord_system);

    // the value of the Dirichlet boundary condition
    auto bc_value = mito::functions::sqrt(x * x + y * y);

    // get all the nodes on the mesh boundary
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, bc_value);

    // loop on all the constrained nodes
    for (const auto & cell : constraints.domain().cells()) {
        // get all the nodes of the cell
        auto nodes = cell.nodes();
        // loop on all the nodes of the cell
        for (const auto & node : nodes) {
            // get the position of the node
            auto position = coord_system.coordinates(node->point());
            // expect that the value of the Dirichlet boundary condition is equal to the radius of
            // the disk up to a reasonable tolerance
            EXPECT_NEAR(bc_value(position), 1.0, 1.e-12);
        }
    }
}

// end of file
