#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(SummitToVTK, Mesh3D)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // read summit mesh
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // write mesh to vtk file
    mito::io::vtk::writer("cube_output", mesh, coord_system);
}