#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(SummitToVTK, Mesh2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // read summit mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh =
        mito::io::summit::reader<mito::topology::simplex_t<2>>(fileStream, geometry, coord_system);

    // write mesh to vtk file
    mito::io::vtk::writer("rectangle_output", mesh, coord_system);
}