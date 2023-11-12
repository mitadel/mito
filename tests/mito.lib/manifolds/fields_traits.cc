#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Manifolds, FieldsTraits)
{
    // a scalar field
    constexpr auto f =
        mito::manifolds::field([](const mito::geometry::coordinates_t<2> & x) -> mito::scalar_t {
            return cos(x[0] * x[1]);
        });
    // assert that {f} is a scalar field
    static_assert(mito::manifolds::is_scalar_field<decltype(f)>);

    // a vector field
    constexpr auto g =
        mito::manifolds::field([](const mito::geometry::coordinates_t<2> & x) -> mito::vector_t<2> {
            return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
        });
    // assert that {g} is a vector field
    static_assert(mito::manifolds::is_vector_field<decltype(g)>);

    // another vector field
    constexpr auto h =
        mito::manifolds::field([](const mito::geometry::coordinates_t<3> & x) -> mito::vector_t<2> {
            return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
        });
    // assert that {h} is also a vector field
    static_assert(mito::manifolds::is_vector_field<decltype(h)>);

    // a second-order tensor field (2x2 identity tensor field in 3 dimensional space)
    constexpr auto i = mito::manifolds::identity_tensor_field<2, 3>;
    // assert that {i} is a tensor field
    static_assert(mito::manifolds::is_tensor_field<decltype(i)>);
    // assert that {i} is a symmetric tensor field
    static_assert(mito::manifolds::is_symmetric_tensor_field<decltype(i)>);
    // assert that {i} is a diagonal tensor field
    static_assert(mito::manifolds::is_diagonal_tensor_field<decltype(i)>);
}


// end of file
