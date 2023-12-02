#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields
static constexpr auto e_x = mito::e_0<2>;
static constexpr auto e_y = mito::e_1<2>;


auto
area(
    const auto & w, mito::topology::topology_t & topology,
    const mito::geometry::geometry_t<2, mito::geometry::EUCLIDEAN> & geometry,
    const mito::topology::vertex_t & v0, const mito::topology::vertex_t & v1,
    const mito::topology::vertex_t & v2) -> mito::scalar_t
{
    // build director segments
    auto segment0 = topology.segment({ v0, v1 });
    auto segment1 = topology.segment({ v1, v2 });

    // build director vectors
    auto director0 = geometry.vector(segment0);
    auto director1 = geometry.vector(segment1);

    // compute volume of triangle
    auto area = 1. / 2. * w(director0, director1);

    // all done
    return area;
}


TEST(Manifolds, EuclideanMetric2D)
{
    // the basis one-forms
    constexpr auto dx = mito::manifolds::one_form(e_x);
    constexpr auto dy = mito::manifolds::one_form(e_y);

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx(e_x) == 1.0);
    static_assert(dx(e_y) == 0.0);
    static_assert(dy(e_x) == 0.0);
    static_assert(dy(e_y) == 1.0);

    // the metric volume element
    constexpr auto w = mito::manifolds::wedge(dx, dy);

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // build nodes of a triangle (counterclockwise order)
    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 0.0, 1.0 });

    // check that even permutations of the vertices give a positive area
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex0, vertex1, vertex2), 0.5);
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex1, vertex2, vertex0), 0.5);
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex2, vertex0, vertex1), 0.5);

    // check that odd permutations of the vertices give a negative area
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex0, vertex2, vertex1), -0.5);
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex2, vertex1, vertex0), -0.5);
    EXPECT_DOUBLE_EQ(area(w, topology, geometry, vertex1, vertex0, vertex2), -0.5);
}


// end of file
