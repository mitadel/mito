#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(SummitToVTK, Mesh2D)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // read summit mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // write mesh to vtk file
    mito::io::vtk::writer("rectangle_output", mesh, coord_system);
}