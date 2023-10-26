#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Fields, VectorFields)
{
    // a point in space
    constexpr mito::geometry::coordinates_t<2> x = { 0.0, 0.0 };

    // a scalar field
    constexpr auto f =
        mito::manifolds::field([](const mito::geometry::coordinates_t<2> & x) -> mito::scalar_t {
            return cos(x[0] * x[1]);
        });

    // check value of field at {x}
    static_assert(f(x) == 1.0);

    // a vector field
    constexpr auto g =
        mito::manifolds::field([](const mito::geometry::coordinates_t<2> & x) -> mito::vector_t<2> {
            return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
        });

    // check value of field at {x}
    static_assert(g(x) == mito::vector_t<2> { 1.0, 1.0 });
}


// end of file
