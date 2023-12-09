#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


auto
volume(
    const auto & w, const mito::geometry::geometry_t<3, mito::geometry::EUCLIDEAN> & geometry,
    const mito::topology::vertex_t & v0, const mito::topology::vertex_t & v1,
    const mito::topology::vertex_t & v2, const mito::topology::vertex_t & v3) -> mito::scalar_t
{
    // build director segments
    auto x0 = geometry.point(v0)->coordinates();
    auto x1 = geometry.point(v1)->coordinates();
    auto x2 = geometry.point(v2)->coordinates();
    auto x3 = geometry.point(v3)->coordinates();

    // build director vectors
    auto director0 = x1 - x0;
    auto director1 = x2 - x0;
    auto director2 = x3 - x0;

    // compute volume of tetrahedron
    auto volume = 1. / 6. * w(director0, director1, director2);

    // all done
    return volume;
}


TEST(Manifolds, EuclideanMetric3D)
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
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex2, vertex3, vertex4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex3, vertex4, vertex2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex4, vertex2, vertex3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex1, vertex4, vertex3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex3, vertex1, vertex4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex4, vertex3, vertex1), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex1, vertex2, vertex4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex2, vertex4, vertex1), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex4, vertex1, vertex2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex1, vertex3, vertex2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex2, vertex1, vertex3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex3, vertex2, vertex1), 1.0 / 6.0);

    // odd permutations of the vertices order result in a negative volume
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex2, vertex4, vertex3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex3, vertex2, vertex4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex1, vertex4, vertex3, vertex2), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex1, vertex3, vertex4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex3, vertex4, vertex1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex2, vertex4, vertex1, vertex3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex1, vertex4, vertex2), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex2, vertex1, vertex4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex3, vertex4, vertex2, vertex1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex1, vertex2, vertex3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex2, vertex3, vertex1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, geometry, vertex4, vertex3, vertex1, vertex2), -1.0 / 6.0);
}


// end of file
