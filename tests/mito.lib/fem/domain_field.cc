// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the x scalar field in 2D
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the y scalar field in 2D
constexpr auto y = mito::functions::component<coordinates_t, 1>;


TEST(Fem, DomainField)
{
    // create a channel
    journal::info_t channel("tests.domain_field");

    // the coordinate system
    auto coord_system = coord_system_t();

    // create a domain field
    auto field = mito::fem::domain_field(x * y);

    // create some nodes
    auto node_0 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 1.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 0.0 });

    // create a geometric simplex
    auto geometric_simplex = mito::geometry::triangle<2>({ node_0, node_1, node_2 });

    // create a mesh with a single triangle
    auto mesh = mito::mesh::mesh<cell_t>();
    mesh.insert({ node_0, node_1, node_2 });

    // create the manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // an isoparametric triangle
    auto element =
        mito::fem::IsoparametricTriangle(geometric_simplex, coord_system, manifold.volume_form());

    // TOFIX: This syntax should also be allowed: field.localize(geometric_simplex).
    // However, in case, the coordinate system should be passed somehow to the function.
    // localize the field on the simplex
    auto localized_field = field.localize(element);

    // evaluate the localized field at the center of the triangle
    auto value = localized_field({ 1.0 / 3.0, 1.0 / 3.0 });

    // check the value
    EXPECT_DOUBLE_EQ(value, (x * y)({ 1.0 / 3.0, 1.0 / 3.0 }));

    // compute the gradient of the localized field with respect to the barycentric coordinates
    auto gradient = mito::fields::gradient(localized_field);

    // evaluate the localized field gradient at the center of the triangle
    auto value_gradient = gradient({ 1.0 / 3.0, 1.0 / 3.0 });

    // check the value of the gradient at the center of the triangle
    EXPECT_DOUBLE_EQ(value_gradient[0], y({ 1.0 / 3.0, 1.0 / 3.0 }));
    EXPECT_DOUBLE_EQ(value_gradient[1], x({ 1.0 / 3.0, 1.0 / 3.0 }));

    // all done
    return;
}


// end of file