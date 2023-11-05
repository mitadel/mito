#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


auto
volume(
    mito::mesh::mesh_t<mito::topology::tetrahedron_t, 3> & mesh,
    const mito::manifolds::manifold_t<
        mito::manifolds::EUCLIDEAN, mito::topology::tetrahedron_t, 3> & manifold,
    const mito::topology::tetrahedron_t & tetrahedron) -> mito::scalar_t
{
    // insert tetrahedron in the mesh
    mesh.insert(tetrahedron);

    // compute the volume of the manifold
    mito::scalar_t result = manifold.volume();

    // erase the tetrahedron
    mesh.erase(tetrahedron);

    // return the volume computed
    return result;
}


TEST(Manifolds, Tetrahedron3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of tetrahedra
    auto mesh = mito::mesh::mesh<mito::topology::tetrahedron_t>(geometry);

    // create a manifold on {mesh} with Euclidean metric
    auto manifold = mito::manifolds::manifold(mesh);

    // get the 0-th basis element for vector fields
    constexpr auto e0 = manifold.e<0>();
    // check that it is equal to [1.0, 0.0, 0.0] at point {0.0, 0.0, 0.0}
    static_assert(e0({ 0.0, 0.0, 0.0 }) == mito::vector_t<3>({ 1.0, 0.0, 0.0 }));

    // get the 1-st basis element for vector fields
    constexpr auto e1 = manifold.e<1>();
    // check that it is equal to [0.0, 1.0, 0.0] at point {0.0, 0.0, 0.0}
    static_assert(e1({ 0.0, 0.0, 0.0 }) == mito::vector_t<3>({ 0.0, 1.0, 0.0 }));

    // get the 2-st basis element for vector fields
    constexpr auto e2 = manifold.e<2>();
    // check that it is equal to [0.0, 0.0, 1.0] at point {0.0, 0.0, 0.0}
    static_assert(e2({ 0.0, 0.0, 0.0 }) == mito::vector_t<3>({ 0.0, 0.0, 1.0 }));

    // get the 0-th basis element for one-form fields
    constexpr auto dx0 = manifold.dx<0>();
    // check that it contracts as expected with the basis vectors
    static_assert(dx0({ 0.0, 0.0, 0.0 })(e0({ 0.0, 0.0, 0.0 })) == 1.0);
    static_assert(dx0({ 0.0, 0.0, 0.0 })(e1({ 0.0, 0.0, 0.0 })) == 0.0);
    static_assert(dx0({ 0.0, 0.0, 0.0 })(e2({ 0.0, 0.0, 0.0 })) == 0.0);

    // get the 1-st basis element for one-form fields
    constexpr auto dx1 = manifold.dx<1>();
    // check that it contracts as expected with the basis vectors
    static_assert(dx1({ 0.0, 0.0, 0.0 })(e0({ 0.0, 0.0, 0.0 })) == 0.0);
    static_assert(dx1({ 0.0, 0.0, 0.0 })(e1({ 0.0, 0.0, 0.0 })) == 1.0);
    static_assert(dx1({ 0.0, 0.0, 0.0 })(e2({ 0.0, 0.0, 0.0 })) == 0.0);

    // get the 2-nd basis element for one-form fields
    constexpr auto dx2 = manifold.dx<2>();
    // check that it contracts as expected with the basis vectors
    static_assert(dx2({ 0.0, 0.0, 0.0 })(e0({ 0.0, 0.0, 0.0 })) == 0.0);
    static_assert(dx2({ 0.0, 0.0, 0.0 })(e1({ 0.0, 0.0, 0.0 })) == 0.0);
    static_assert(dx2({ 0.0, 0.0, 0.0 })(e2({ 0.0, 0.0, 0.0 })) == 1.0);

    // build nodes
    auto vertex1 = geometry.node({ 0.0, 0.0, 0.0 });
    auto vertex2 = geometry.node({ 1.0, 0.0, 0.0 });
    auto vertex3 = geometry.node({ 0.0, 1.0, 0.0 });
    auto vertex4 = geometry.node({ 0.0, 0.0, 1.0 });

    // build tetrahedron with a positive volume (reference tetrahedron)
    auto tetrahedron = topology.tetrahedron({ vertex1, vertex2, vertex3, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex2, vertex3, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex3, vertex4, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex4, vertex2, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex1, vertex4, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex3, vertex1, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex4, vertex3, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex1, vertex2, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex2, vertex4, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex4, vertex1, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex1, vertex3, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex2, vertex1, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex3, vertex2, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex2, vertex4, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex3, vertex2, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex1, vertex4, vertex3, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex1, vertex3, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex3, vertex4, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex2, vertex4, vertex1, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex1, vertex4, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex2, vertex1, vertex4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex3, vertex4, vertex2, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex1, vertex2, vertex3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex2, vertex3, vertex1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = topology.tetrahedron({ vertex4, vertex3, vertex1, vertex2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(volume(mesh, manifold, tetrahedron), -1.0 / 6.0);
}


// end of file
