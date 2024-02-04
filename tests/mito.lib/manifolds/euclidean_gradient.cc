#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields
static constexpr auto e_0 = mito::manifolds::uniform_field<2>(mito::e_0<2>);
static constexpr auto e_1 = mito::manifolds::uniform_field<2>(mito::e_1<2>);

// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;


TEST(Manifolds, EuclideanGradient)
{
    // a scalar field
    [[maybe_unused]] auto f = mito::manifolds::field(
        [](const coordinates_t & x) -> mito::scalar_t { return x[0] * x[1]; });

    // df/dx[0]
    constexpr auto df0 =
        mito::manifolds::field([](const coordinates_t & x) -> mito::scalar_t { return x[1]; });

    // df/dx[1]
    constexpr auto df1 =
        mito::manifolds::field([](const coordinates_t & x) -> mito::scalar_t { return x[0]; });

    // the Euclidean metric
    constexpr auto g = mito::manifolds::metric<mito::geometry::EUCLIDEAN, 2, 2>::field();

    // the gradient form
    constexpr auto gradient = mito::manifolds::one_form(df0 * e_0 + df1 * e_1, g);

    // a point in space
    constexpr auto x = mito::geometry::coordinates({ 1.0, 1.0 });

    // check that the contractions of the gradient form with the basis vectors return the partial
    // derivatives of {f}
    static_assert(gradient(x)(e_0(x)) == df0(x));
    static_assert(gradient(x)(e_1(x)) == df1(x));
}


// end of file
