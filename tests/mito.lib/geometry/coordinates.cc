#include <gtest/gtest.h>
// TOFIX
// #include <mito/geometry.h>
#include "mito/base.h"
#include "mito/geometry/forward.h"
#include "mito/geometry/Coordinates.h"
#include "mito/geometry/algebra_coordinates.h"

// coordinates alias in D-dimensions
template <int D, mito::geometry::CoordinateSystem coordT>
using coordinates_t = mito::geometry::Coordinates<D, coordT>;

// factory for coordinates from brace-enclosed initializer list
template <mito::geometry::CoordinateSystem coordT = mito::geometry::EUCLIDEAN, int D>
constexpr auto
coordinates(const mito::scalar_t (&&coords)[D])
{
    return coordinates_t<D, coordT>(coords);
}

TEST(Coordinates, Euclidean)
{
    // create two sets of coordinates
    constexpr auto coord_A = coordinates({ 0.0, 1.0 });
    constexpr auto coord_B = coordinates({ 1.0, 0.0 });

    // the exact value of {coord_A + coord_B} / 2
    constexpr auto coord_C = coordinates({ 0.5, 0.5 });

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
    constexpr auto coord_A = coordinates<mito::geometry::POLAR>({ 1.0, 0.0 });
    constexpr auto coord_B = coordinates<mito::geometry::POLAR>({ 1.0, 0.5 * std::numbers::pi });

    // the exact value of {coord_A + coord_B} / 2.0
    constexpr auto coord_C =
        coordinates<mito::geometry::POLAR>({ std::sqrt(2.0) / 2.0, 0.25 * std::numbers::pi });

    // compute {coord_A + coord_B} / 2.0
    constexpr auto coord_D = (coord_A + coord_B) / 2.0;

    // check that the result of the average is correct
    static_assert(coord_C == coord_D);

    // check that the result of the average is correct
    static_assert(mito::geometry::distance(coord_D, coord_C) == 0.0);
}


// end of file