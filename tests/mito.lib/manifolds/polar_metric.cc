#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Metric, Polar)
{
    // the metric field
    constexpr auto g =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field(
            [](const mito::geometry::coordinates_t<2> & x) -> mito::diagonal_matrix_t<2> {
                // e_r + r^2 * e_theta
                return mito::e_00<2> + (x[0] * x[0]) * mito::e_11<2>;
            });

    // the inverse metric field
    constexpr auto g_inv =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field(
            [](const mito::geometry::coordinates_t<2> & x) -> mito::diagonal_matrix_t<2> {
                // e_r + 1/r^2 * e_theta
                return mito::e_00<2> + 1.0 / (x[0] * x[0]) * mito::e_11<2>;
            });

    // the basis one forms
    constexpr auto dr =
        mito::manifolds::one_form(mito::manifolds::uniform_field<2>(mito::e_0<2>), g_inv);
    constexpr auto dtheta =
        mito::manifolds::one_form(mito::manifolds::uniform_field<2>(mito::e_1<2>), g_inv);

    // the metric volume element
    constexpr auto w =
        mito::manifolds::sqrt(mito::manifolds::determinant(g)) * mito::manifolds::wedge(dr, dtheta);
}


// end of file
