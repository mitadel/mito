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

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto point0 = point_cloud.point({ 0.0, 0.0 });
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    auto point3 = point_cloud.point({ 0.0, 1.0 });

    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();

    // add nodes to geometry
    geometry.node(vertex0, point0);
    geometry.node(vertex1, point1);
    geometry.node(vertex2, point2);
    geometry.node(vertex3, point3);

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
    auto simplex_pair = mito::mesh::flipDiagonal(std::make_pair(cell0, cell1));

    mesh.insert(simplex_pair.first);
    mesh.erase(std::move(cell0));

    mesh.insert(simplex_pair.second);
    mesh.erase(std::move(cell1));

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = tetra(mesh, geometry, subdivisions);

    // create manifold from the mesh
    auto manifold = mito::manifolds::manifold(tetra_mesh);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;

    // all done
    return 0;
}