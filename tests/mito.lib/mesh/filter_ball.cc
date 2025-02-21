#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Mesh, FilterBall)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a ball
    std::ifstream fileStream("ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // mesh faces
    auto mesh_faces = mito::mesh::filter<2>(mesh);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("ball_internal_faces", mesh_faces, coord_system).write();
#endif

    // mesh wireframe
    auto wireframe = mito::mesh::filter<1>(mesh);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("ball_wireframe", wireframe, coord_system).write();
#endif

    // mesh wireframe
    auto mesh_wireframe_from_internal_faces = mito::mesh::filter<1>(mesh_faces);

    // check that the two wireframe meshes have the same number of cells (they should be identical)
    EXPECT_EQ(wireframe.nCells(), mesh_wireframe_from_internal_faces.nCells());

    // fetch the boundary of the ball (a sphere)
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // boundary mesh wireframe
    auto boundary_wireframe = mito::mesh::filter<1>(boundary_mesh);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("sphere_wireframe", boundary_wireframe, coord_system).write();
#endif
}
