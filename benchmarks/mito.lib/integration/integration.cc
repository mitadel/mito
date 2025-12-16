// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the {x_0} components of a 2D vector
constexpr auto x_0 = mito::geometry::cartesian::x_0<2>;
// the function extracting the {x_1} components of a 2D vector
constexpr auto x_1 = mito::geometry::cartesian::x_1<2>;


int
main()
{
    // make a channel
    journal::info_t channel("benchmarks.integration");

    /**
         * Mesh with four cells:
            (0,1)           (1,1)
              4               2
              +---------------+
              | .           . |
              |   .       .   |
              |     .   .     |
              |       . 3     |
              |     .   .     |
              |   .       .   |
              | .           . |
              +---------------+
              0               1
            (0,0)           (1,0)
        */

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert cells in the mesh
    auto & cell0 = mesh.insert({ node_0, node_1, node_3 });
    auto & cell1 = mesh.insert({ node_1, node_2, node_3 });

    // flip the common edge of the two triangles
    auto [new_cell0, new_cell1] = mito::geometry::flip_diagonal<2>({ cell0, cell1 });

    mesh.insert(new_cell0);
    mesh.erase(cell0);
    mesh.insert(new_cell1);
    mesh.erase(cell1);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);

    // create manifold from the mesh
    auto manifold = mito::manifolds::manifold(tetra_mesh, coord_system);

    // instantiate a scalar field
    auto f = mito::functions::cos(x_0 * x_1);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f);
    auto exact = mito::tensor::scalar_t{ 0.9460830607878437 };
    channel << "Integration of cos(x*y): Result = " << result
            << ", Error = " << std::fabs(result - exact) << journal::endl;

    // all done
    return 0;
}