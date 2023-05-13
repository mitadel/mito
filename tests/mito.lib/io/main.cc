#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(MeshReader, LoadSummitSegmentsMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    std::ifstream fileStream("segments.summit");
    auto mesh = mito::reader::summit<mito::topology::segment_t>(fileStream, geometry);

    // assert you read 10 cells
    EXPECT_EQ(mesh.nCells(), 10);

    // assert you found 2 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 2);
    }

    // show me the cells
    std::cout << "Initial mesh: " << std::endl;
    int count = 0;
    mito::topology::segment_t segment;
    for (const auto & simplex : mesh.cells()) {
        ++count;
        if (count == 5) {
            segment = simplex;
        }
    }

    // erase a simplex
    mesh.erase(segment);
    topology.erase(segment);

    // assert you read 9 cells
    EXPECT_EQ(mesh.nCells(), 9);

    // assert you found 4 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }
}

TEST(MeshReader, LoadSummitMesh2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::reader::summit<mito::topology::simplex_t<2>>(fileStream, geometry);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_mesh = mesh.boundary();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}

// TODO: add test on triangle in 3D

TEST(MeshReader, LoadSummitMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::reader::summit<mito::topology::simplex_t<3>>(fileStream, geometry);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;
}

#ifdef WITH_VTK
TEST(MeshWriter, Vtk)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 3D
    auto mesh = mito::mesh::mesh<mito::topology::tetrahedron_t>(geometry);

    // vertices for one single tetrahedron
    auto & vertex0 = geometry.node({ 0.0, 0.0, 0.0 });
    auto & vertex1 = geometry.node({ 1.0, 0.0, 0.0 });
    auto & vertex2 = geometry.node({ 0.0, 1.0, 0.0 });
    auto & vertex3 = geometry.node({ 0.0, 0.0, 1.0 });

    // build the tetrahedron
    const auto & cell = topology.tetrahedron({ vertex0, vertex1, vertex2, vertex3 });

    // insert cell in the mesh
    mesh.insert(cell);

    // use tetra
    auto tetra_mesh = tetra(mesh, geometry);

    // write mesh to vtk file
    mito::writer::vtk("output", tetra_mesh);
}

TEST(MeshWriter, SummitMeshToVtk)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read summit mesh
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::reader::summit<mito::topology::simplex_t<3>>(fileStream, geometry);

    // write mesh to vtk file
    mito::writer::vtk("output", mesh);
}
#endif