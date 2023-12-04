#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Coordinates, Euclidean)
{
    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::coordinates<mito::geometry::EUCLIDEAN>({ 0.0, 1.0 });
    constexpr auto coord_B = mito::geometry::coordinates<mito::geometry::EUCLIDEAN>({ 1.0, 0.0 });

    // the exact value of {coord_A + coord_B} / 2
    constexpr auto coord_C = mito::geometry::coordinates<mito::geometry::EUCLIDEAN>({ 0.5, 0.5 });

    // compute {coord_A + coord_B} / 2.0
    constexpr auto coord_D = (coord_A + coord_B) / 2.0;

    // check that the result of the average is correct
    static_assert(coord_C == coord_D);

    // check that the result of the average is correct
    static_assert(mito::geometry::distance(coord_D, coord_C) == 0.0);
}


TEST(Coordinates, POLAR)
{
    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::coordinates<mito::geometry::POLAR>({ 1.0, 0.0 });
    constexpr auto coord_B =
        mito::geometry::coordinates<mito::geometry::POLAR>({ 1.0, 0.5 * std::numbers::pi });

    // the exact value of {coord_A + coord_B} / 2.0
    constexpr auto coord_C = mito::geometry::coordinates<mito::geometry::POLAR>(
        { std::sqrt(2.0) / 2.0, 0.25 * std::numbers::pi });

    // compute {coord_A + coord_B} / 2.0
    constexpr auto coord_D = (coord_A + coord_B) / 2.0;

    // check that the result of the average is correct
    static_assert(coord_C == coord_D);

    // check that the result of the average is correct
    static_assert(mito::geometry::distance(coord_D, coord_C) == 0.0);
}


// end of file
