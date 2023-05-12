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
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    auto point0 = point_cloud.point({ 0.0, 0.0 });
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    auto point3 = point_cloud.point({ 0.5, 0.5 });
    auto point4 = point_cloud.point({ 0.0, 1.0 });

    auto & vertex0 = topology.vertex();
    auto & vertex1 = topology.vertex();
    auto & vertex2 = topology.vertex();
    auto & vertex3 = topology.vertex();
    auto & vertex4 = topology.vertex();

    // add nodes to geometry
    geometry.node(vertex0, point0);
    geometry.node(vertex1, point1);
    geometry.node(vertex2, point2);
    geometry.node(vertex3, point3);
    geometry.node(vertex4, point4);

    auto & segment0 = topology.segment({ vertex0, vertex1 });
    auto & segment1 = topology.segment({ vertex1, vertex3 });
    auto & segment2 = topology.segment({ vertex3, vertex0 });
    auto & cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto & segment3 = topology.segment({ vertex1, vertex2 });
    auto & segment4 = topology.segment({ vertex2, vertex3 });
    auto & segment5 = topology.segment({ vertex3, vertex1 });
    auto & cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto & segment6 = topology.segment({ vertex2, vertex4 });
    auto & segment7 = topology.segment({ vertex4, vertex3 });
    auto & segment8 = topology.segment({ vertex3, vertex2 });
    auto & cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto & segment9 = topology.segment({ vertex4, vertex0 });
    auto & segment10 = topology.segment({ vertex0, vertex3 });
    auto & segment11 = topology.segment({ vertex3, vertex4 });
    auto & cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells(), 4);

    // assert you found 4 cells (segments) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }

    // TOFIX: 8 vertices are found on the boundary due to repeated entries with different
    //          orientations...
    // assert you found 4 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary<0>();
        EXPECT_EQ(boundary_mesh.nCells(), 8);
    }

    return;
}

TEST(Mesh, LoadSummitSegmentsMesh3D)
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

TEST(Mesh, LoadSummitMesh2D)
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

TEST(Mesh, LoadSummitMesh3D)
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

TEST(Tetra, MeshSegment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 1D
    auto & point_cloud = mito::geometry::point_cloud<1>();

    // a 1D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);

    // build nodes
    auto & vertex0 = geometry.node({ 0.0 });
    auto & vertex1 = geometry.node({ 1.0 });

    // build segment
    auto & segment_a = topology.segment({ vertex0, vertex1 });

    // insert cells in the mesh
    mesh.insert(segment_a);

    EXPECT_EQ(mesh.nCells(), 1);

    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, 3);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}

TEST(Tetra, MeshTwoTriangles)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto & vertex0 = geometry.node({ 0.0, 0.0 });
    auto & vertex1 = geometry.node({ 1.0, 0.0 });
    auto & vertex2 = geometry.node({ 1.0, 1.0 });
    auto & vertex3 = geometry.node({ 0.0, 1.0 });

    // build segments
    auto & segment_a = topology.segment({ vertex0, vertex1 });
    auto & segment_b = topology.segment({ vertex1, vertex2 });
    auto & segment_c = topology.segment({ vertex2, vertex3 });
    auto & segment_d = topology.segment({ vertex3, vertex0 });
    auto & segment_e = topology.segment({ vertex0, vertex2 });
    auto & segment_e_flip = topology.flip(segment_e);

    // build triangles
    auto & simplex0 = topology.triangle({ segment_a, segment_b, segment_e_flip });
    auto & simplex1 = topology.triangle({ segment_e, segment_c, segment_d });

    // insert cells in the mesh
    mesh.insert(simplex0);
    mesh.insert(simplex1);

    EXPECT_EQ(mesh.nCells(), 2);

    auto tetra_mesh = mito::mesh::tetra(mesh, geometry);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}

TEST(Tetra, MeshRectangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::reader::summit<mito::topology::triangle_t>(fileStream, geometry);

    // count the cells of the mesh
    int cells = mesh.nCells();

    // do one tetra mesh refinement
    auto tetra_mesh = tetra(mesh, geometry);
    // assert that the refined mesh has 4 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);

    // do two tetra mesh refinements on the original mesh
    auto tetra_tetra_mesh_original = tetra(mesh, geometry, 2);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_original.nCells(), 16 * cells);

    // do a tetra mesh refinement on the refined mesh
    auto tetra_tetra_mesh_refined = tetra(tetra_mesh, geometry);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_refined.nCells(), 16 * cells);
}

TEST(Tetra, MeshRectangleArea)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::reader::summit<mito::topology::triangle_t>(fileStream, geometry);

    // do tetra mesh refinements
    auto tetra_mesh = tetra(mesh, geometry, 1);

    // compute the volume of the original mesh
    auto volume_mesh = mito::quadrature::volume(mito::manifolds::manifold(mesh));

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::quadrature::volume(mito::manifolds::manifold(tetra_mesh));

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-15);
}

TEST(Tetra, CubeVolume)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::reader::summit<mito::topology::simplex_t<3>>(fileStream, geometry);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = tetra(mesh, geometry, subdivisions);
    // assert that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());

    // compute the volume of the original mesh
    auto volume_mesh = mito::quadrature::volume(mito::manifolds::manifold(mesh));

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::quadrature::volume(mito::manifolds::manifold(tetra_mesh));

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-13);
}

TEST(Tetra, ZeroSubdivisions)
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

    // generate tetra mesh with 0 subdivisions
    EXPECT_EXIT(
        {
            tetra(mesh, geometry, 0);
            exit(0);
        },
        ::testing::ExitedWithCode(0), ".*");
}

TEST(Mesh, TetraMeshVtkWriter)
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

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::writer::vtk("output", tetra_mesh);
#endif
}

TEST(Mesh, SummitMeshVtkWriter)
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

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::writer::vtk("output", mesh);
#endif
}