#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(VtkWriter, WriteSummitMesh2DToVtk)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read summit mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::topology::simplex_t<2>>(fileStream, geometry);

    // write mesh to vtk file
    mito::io::vtk::writer("rectangle_output", mesh);
}