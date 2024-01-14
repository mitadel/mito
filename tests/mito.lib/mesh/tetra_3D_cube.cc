#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


TEST(Tetra, Cube)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::io::summit::reader<mito::topology::simplex_t<3>>(fileStream, geometry);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, subdivisions);
    // assert that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh).volume();

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-13);
}
