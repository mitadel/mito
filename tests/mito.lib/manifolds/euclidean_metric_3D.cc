#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


auto
volume(
    const auto & w, mito::topology::topology_t & topology,
    const mito::geometry::geometry_t<3> & geometry, const mito::topology::vertex_t & v0,
    const mito::topology::vertex_t & v1, const mito::topology::vertex_t & v2,
    const mito::topology::vertex_t & v3) -> mito::scalar_t
{
    // build director segments
    auto segment0 = topology.segment({ v0, v1 });
    auto segment1 = topology.segment({ v1, v2 });
    auto segment2 = topology.segment({ v0, v3 });

    // build director vectors
    auto director0 = geometry.vector(segment0);
    auto director1 = geometry.vector(segment1);
    auto director2 = geometry.vector(segment2);

    // compute volume of tetrahedron
    auto volume = 1. / 3. * w(director0, director1, director2);

    // all done
    return volume;
}


TEST(Manifolds, EuclideanMetric2D)
{
    // the basis one-forms
    constexpr auto dx = mito::manifolds::one_form(e_x);
    constexpr auto dy = mito::manifolds::one_form(e_y);
    constexpr auto dz = mito::manifolds::one_form(e_z);

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx(e_x) == 1.0);
    static_assert(dx(e_y) == 0.0);
    static_assert(dx(e_z) == 0.0);
    static_assert(dy(e_x) == 0.0);
    static_assert(dy(e_y) == 1.0);
    static_assert(dy(e_z) == 0.0);
    static_assert(dz(e_x) == 0.0);
    static_assert(dz(e_y) == 0.0);
    static_assert(dz(e_z) == 1.0);

    // the metric volume element
    constexpr auto w = mito::manifolds::wedge(dx, dy, dz);

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // build nodes of a triangle (counterclockwise order)
    auto vertex1 = geometry.node({ 0.0, 0.0, 0.0 });
    auto vertex2 = geometry.node({ 1.0, 0.0, 0.0 });
    auto vertex3 = geometry.node({ 0.0, 1.0, 0.0 });
    auto vertex4 = geometry.node({ 0.0, 0.0, 1.0 });

    // even permutations of the vertices order result in a positive volume
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex2, vertex3, vertex4), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex3, vertex4, vertex2), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex4, vertex2, vertex3), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex1, vertex4, vertex3), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex3, vertex1, vertex4), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex4, vertex3, vertex1), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex1, vertex2, vertex4), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex2, vertex4, vertex1), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex4, vertex1, vertex2), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex1, vertex3, vertex2), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex2, vertex1, vertex3), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex3, vertex2, vertex1), 1.0 / 3.0);

    // odd permutations of the vertices order result in a positive volume
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex2, vertex4, vertex3), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex3, vertex2, vertex4), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex1, vertex4, vertex3, vertex2), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex1, vertex3, vertex4), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex3, vertex4, vertex1), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex2, vertex4, vertex1, vertex3), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex1, vertex4, vertex2), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex2, vertex1, vertex4), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex3, vertex4, vertex2, vertex1), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex1, vertex2, vertex3), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex2, vertex3, vertex1), -1.0 / 3.0);
    EXPECT_DOUBLE_EQ(volume(w, topology, geometry, vertex4, vertex3, vertex1, vertex2), -1.0 / 3.0);
}


// end of file
