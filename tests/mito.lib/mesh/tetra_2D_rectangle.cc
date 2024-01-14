#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


TEST(Tetra, Rectangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t>(fileStream, geometry);

    // do tetra mesh refinements
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, 1);

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh).volume();

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-15);
}
