#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


auto
volume(
    mito::mesh::mesh_t<mito::topology::triangle_t, 2, mito::geometry::EUCLIDEAN> & mesh,
    const mito::manifolds::manifold_t<mito::geometry::EUCLIDEAN, mito::topology::triangle_t, 2> &
        manifold,
    const mito::topology::triangle_t & triangle) -> mito::scalar_t
{
    // insert triangle in the mesh
    mesh.insert(triangle);

    // compute the volume of the manifold
    mito::scalar_t result = manifold.volume();

    // erase the triangle
    mesh.erase(triangle);

    // return the volume computed
    return result;
}


TEST(Manifolds, Triangle2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 0.0, 1.0 });

    // create a manifold on {mesh} with Euclidean metric
    auto manifold = mito::manifolds::manifold(mesh);

    // build triangle with a positive volume (reference triangle)
    auto triangle = topology.triangle({ vertex0, vertex1, vertex2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = topology.triangle({ vertex1, vertex2, vertex0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = topology.triangle({ vertex2, vertex0, vertex1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), 0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = topology.triangle({ vertex0, vertex2, vertex1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = topology.triangle({ vertex1, vertex0, vertex2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = topology.triangle({ vertex2, vertex1, vertex0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, triangle), -0.5);
}


// end of file
