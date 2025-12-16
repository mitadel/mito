// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// simplicial cells in 2D
using cell_t = mito::geometry::triangle_t<2>;
// first degree finite elements
constexpr int degree = 1;
// assemble the finite element type
using finite_element_t = mito::fem::isoparametric_simplex_t<degree, cell_t>;
// the x scalar field in 2D
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the y scalar field in 2D
constexpr auto y = mito::functions::component<coordinates_t, 1>;


TEST(Fem, FemField)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a square in 2D
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, coord_system);

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // TOFIX: it should not be mandatory to set constraints to create a function space, let's remove
    // this bit once we implement constraints properly
    // get the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the zero field
    auto zero = mito::functions::zero<coordinates_t>;

    // set homogeneous Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // the function space (linear elements on the manifold)
    auto function_space = mito::fem::function_space<finite_element_t>(manifold, constraints);

    // get the map between mesh nodes and discretization nodes
    const auto & node_map = function_space.node_map();

    // get a scalar-valued fem field on the function space
    auto fem_field = function_space.fem_field<mito::tensor::scalar_t>("linear field");

    // create a continuous linear field
    auto field = x + y;

    // loop on all the elements of the functions space
    for (const auto & element : function_space.elements()) {
        // loop on all the nodes of the element
        for (const auto & node : element.cell().nodes()) {
            // compute the coordinates of the node
            auto coords = coord_system.coordinates(node->point());
            // set the field value at {node}
            fem_field(node_map.at(node)) = field(coords);
        }
    }

    // loop on all the elements of the functions space
    for (const auto & element : function_space.elements()) {
        // localize the field on {element}
        auto local = fem_field.localize(element);
        // get the coordinates of the center of the element
        auto center_coords = mito::geometry::barycenter(element.cell(), coord_system);
        // check that the results is the same regardless of whether we evaluate the localized field
        // or the original continuous field
        EXPECT_DOUBLE_EQ(local({ 1.0 / 3.0, 1.0 / 3.0 }), field(center_coords));
    }

    // all done
    return;
}