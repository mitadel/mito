#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


auto
volume(
    mito::mesh::mesh_t<mito::topology::triangle_t, 2> & mesh,
    const mito::manifolds::manifold_t<mito::manifolds::EUCLIDEAN, mito::topology::triangle_t, 2> &
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

    // get the 0-th basis element for vector fields
    constexpr auto e0 = manifold.e<0>();
    // check that it is equal to [1.0, 0.0] at point {0.0, 0.0}
    static_assert(e0({ 0.0, 0.0 }) == mito::vector_t<2>({ 1.0, 0.0 }));

    // get the 1-st basis element for vector fields
    constexpr auto e1 = manifold.e<1>();
    // check that it is equal to [0.0, 1.0] at point {0.0, 0.0}
    static_assert(e1({ 0.0, 0.0 }) == mito::vector_t<2>({ 0.0, 1.0 }));

    // get the 0-th basis element for one-form fields
    constexpr auto dx0 = manifold.dx<0>();
    // check that it contracts as expected with the basis vectors
    static_assert(dx0({ 0.0, 0.0 })(e0({ 0.0, 0.0 })) == 1.0);
    static_assert(dx0({ 0.0, 0.0 })(e1({ 0.0, 0.0 })) == 0.0);

    // get the 1-st basis element for one-form fields
    constexpr auto dx1 = manifold.dx<1>();
    // check that it contracts as expected with the basis vectors
    static_assert(dx1({ 0.0, 0.0 })(e0({ 0.0, 0.0 })) == 0.0);
    static_assert(dx1({ 0.0, 0.0 })(e1({ 0.0, 0.0 })) == 1.0);

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
