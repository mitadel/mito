#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields (e_r and e_theta)
static constexpr auto e_r = mito::manifolds::uniform_field<2>(mito::e_0<2>);
static constexpr auto e_t = mito::manifolds::uniform_field<2>(mito::e_1<2>);

// the basis for diagonal second-order tensor fields (e_rr and e_thetatheta)
static constexpr auto e_rr = mito::manifolds::uniform_field<2>(mito::e_00<2>);
static constexpr auto e_tt = mito::manifolds::uniform_field<2>(mito::e_11<2>);


TEST(Metric, Polar)
{
    // the metric field
    constexpr auto g =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field(
            [](const mito::geometry::coordinates_t<2> & x) -> mito::diagonal_matrix_t<2> {
                // e_rr + r^2 * e_tt
                return e_rr(x) + (x[0] * x[0]) * e_tt(x);
            });

    // the inverse metric field
    constexpr auto g_inv =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field(
            [](const mito::geometry::coordinates_t<2> & x) -> mito::diagonal_matrix_t<2> {
                // e_r + 1/r^2 * e_theta
                return e_rr(x) + 1.0 / (x[0] * x[0]) * e_tt(x);
            });

    // the basis one-forms
    constexpr auto dr = mito::manifolds::one_form(g_inv * e_r, g);
    constexpr auto dt = mito::manifolds::one_form(g_inv * e_t, g);


    // the metric volume element
    constexpr auto w =
        mito::manifolds::sqrt(mito::manifolds::determinant(g)) * mito::manifolds::wedge(dr, dt);

}


// end of file
