#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Manifolds, Euclidean3D)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // an empty mesh of tetrahedra
    auto mesh = mito::mesh::mesh<mito::geometry::tetrahedron_t<3>>();

    // create a manifold on {mesh} with Euclidean metric
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

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
}


// end of file
