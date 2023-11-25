#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Manifolds, Metric)
{
    // a point in space
    constexpr mito::scalar_t a = 2.0;
    constexpr auto point = mito::geometry::coordinates({ a, 0.0 });

    // the Euclidean metric
    constexpr auto euclidean_metric =
        mito::manifolds::metric<mito::geometry::EUCLIDEAN, 2, 2>::field();
    // check that the metric field at a point is the identity
    static_assert(euclidean_metric(point) == mito::diagonal_matrix_t<2>({ 1.0, 1.0 }));

    // the polar metric
    constexpr auto polar_metric = mito::manifolds::metric<mito::geometry::POLAR, 2, 2>::field();
    // check that the metric field at a point is e_00 + a^2 e_11
    static_assert(polar_metric(point) == mito::diagonal_matrix_t<2>({ 1.0, a * a }));
}


// end of file
