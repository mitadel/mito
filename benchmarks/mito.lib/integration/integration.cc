// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito/mito.h>

int
main()
{
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

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert cells in the mesh
    auto cell0 = mesh.insert({ node_0, node_1, node_3 });
    auto cell1 = mesh.insert({ node_1, node_2, node_3 });

    // flip the common edge of the two triangles
    auto simplex_pair = mito::geometry::flip_diagonal<2>({ *cell0, *cell1 });

    mesh.insert(simplex_pair.first);
    mesh.erase(cell0);
    mesh.insert(simplex_pair.second);
    mesh.erase(cell1);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);

    // create manifold from the mesh
    auto manifold = mito::manifolds::manifold(tetra_mesh, coord_system);

    // instantiate a scalar field
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;
    auto f = mito::manifolds::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f);
    auto exact = mito::scalar_t(0.9460830607878437);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;

    // all done
    return 0;
}