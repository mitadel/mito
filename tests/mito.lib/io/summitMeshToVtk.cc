#include <gtest/gtest.h>
#include <mito/mito.h>


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
    auto mesh = mito::io::mesh::reader<mito::topology::simplex_t<3>>(fileStream, geometry);

    // write mesh to vtk file
    mito::io::mesh::writer("cube_output", mesh);
}