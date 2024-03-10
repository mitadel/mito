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

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto vertex0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 1.0, 1.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

    // build segments and cells
    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    // insert cells in the mesh
    mesh.insert(cell0);
    mesh.insert(cell1);

    // flip the common edge of the two triangles
    auto simplex_pair = mito::topology::flipDiagonal(std::make_pair(cell0, cell1));

    mesh.insert(simplex_pair.first);
    mesh.erase(cell0);
    topology.erase(cell0);

    mesh.insert(simplex_pair.second);
    mesh.erase(cell1);
    topology.erase(cell1);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system, subdivisions);

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