#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Barycenter, Triangle2D)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // build three nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.5, 0.5 });

    // build a triangle embedded in 2D
    auto triangle = mito::geometry::triangle<2>({ node_0, node_1, node_2 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(triangle, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.5, 1.0 / 6.0 }));

    // all done
    return;
}


// end of file
