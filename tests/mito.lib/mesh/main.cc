#include <gtest/gtest.h>
#include <mito/mito.h>

TEST(Mesh, BuildMesh)
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
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<2, mito::topology::simplex_t>();

    auto vertex0 = topology.vertex();
    auto point0 = point_cloud.point({ 0.0, 0.0 });
    mesh.insert(vertex0, point0);
    auto vertex1 = topology.vertex();
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    mesh.insert(vertex1, point1);
    auto vertex2 = topology.vertex();
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    mesh.insert(vertex2, point2);
    auto vertex3 = topology.vertex();
    auto point3 = point_cloud.point({ 0.5, 0.5 });
    mesh.insert(vertex3, point3);
    auto vertex4 = topology.vertex();
    auto point4 = point_cloud.point({ 0.0, 1.0 });
    mesh.insert(vertex4, point4);

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex4, vertex3 });
    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex0 });

    auto cell0 = topology.triangle({ segment0, segment1, segment2 });
    auto cell1 = topology.triangle({ segment3, segment4, segment1 });
    auto cell2 = topology.triangle({ segment6, segment5, segment4 });
    auto cell3 = topology.triangle({ segment7, segment2, segment5 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    return;
}

TEST(Mesh, LoadSummitMesh2D)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_cells = mito::mesh::boundary<1>(mesh, topology);
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}

TEST(Mesh, LoadSummitMesh3D)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<3>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::mesh::summit<3, mito::topology::simplex_t>(fileStream, topology, point_cloud);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;
}

TEST(Mesh, QuadratureOnMesh)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);
    // TOFIX: is it better that the cells and vertices are first fetched from mesh and then
    //  used to build the manifold?
    // instantiate a cell set as a collection of simplices and vertices.
    // const auto & cells = mesh.cells<2>();    // TODO: region label to fetch cells
    // auto manifold = mito::manifolds::manifold<2>(cells);
    auto manifold = mito::manifolds::manifold<2>(mesh);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    EXPECT_NEAR(result, 0.946083, 1.e-7);
}
