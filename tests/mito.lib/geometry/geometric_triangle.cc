#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Geometry, GeometricTriangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & cloud = mito::geometry::point_cloud<2>();

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // build points
    auto point_a = cloud.point();
    auto point_b = cloud.point();
    auto point_c = cloud.point();

    // place points
    coord_system.place(point_a, { 0.0, 0.0 });
    coord_system.place(point_b, { 1.0, 0.0 });
    coord_system.place(point_c, { 0.0, 1.0 });

    // build vertices
    auto vertex_0 = topology.vertex();
    auto vertex_1 = topology.vertex();
    auto vertex_2 = topology.vertex();

    // build triangle with a combination of vertices
    // TOFIX: reserve the words {segment}, {triangle}, {tetrahedron} for the geometric entities
    auto simplex = topology.triangle({ vertex_0, vertex_1, vertex_2 });

    // build a geometric with a combination of vertices
    // TOFIX: implement the following:
    // auto triangle = geometry.triangle(
    // simplex, { { vertex_0, point_a }, { vertex_1, point_b }, { vertex_2, point_c } });
    auto triangle = mito::geometry::GeometricSimplex<2, 2>(
        simplex, { mito::geometry::node_t<2> { vertex_0, point_a },
                   mito::geometry::node_t<2> { vertex_1, point_b },
                   mito::geometry::node_t<2> { vertex_2, point_c } });
}
