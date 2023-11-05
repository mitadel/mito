#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


// the type for {composition_t}
template <int N>
using composition_t = std::array<int, N>;
// namespace specification
using mito::math::permutation_sign;


TEST(Math, PermutationSign)
{
    {
        // reference permutation 2D
        constexpr auto reference = composition_t<2> { 1, 2 };

        // odd permutation
        static_assert(permutation_sign(composition_t<2> { 2, 1 }, reference) == -1);
    }

    {
        // reference permutation 3D
        constexpr auto reference = composition_t<3> { 1, 2, 3 };

        // even permutations
        static_assert(permutation_sign(composition_t<3> { 2, 3, 1 }, reference) == +1);
        static_assert(permutation_sign(composition_t<3> { 3, 1, 2 }, reference) == +1);

        // odd permutations
        static_assert(permutation_sign(composition_t<3> { 1, 3, 2 }, reference) == -1);
        static_assert(permutation_sign(composition_t<3> { 3, 2, 1 }, reference) == -1);
        static_assert(permutation_sign(composition_t<3> { 2, 1, 3 }, reference) == -1);
    }

    {
        // reference permutation 4D
        constexpr auto reference = composition_t<4> { 1, 2, 3, 4 };

        // even permutations
        static_assert(permutation_sign(composition_t<4> { 1, 2, 3, 4 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 1, 3, 4, 2 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 1, 4, 2, 3 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 2, 1, 4, 3 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 2, 3, 1, 4 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 2, 4, 3, 1 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 3, 1, 2, 4 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 3, 2, 4, 1 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 3, 4, 1, 2 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 4, 1, 3, 2 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 4, 2, 1, 3 }, reference) == +1);
        static_assert(permutation_sign(composition_t<4> { 4, 3, 2, 1 }, reference) == +1);

        // odd permutations
        static_assert(permutation_sign(composition_t<4> { 1, 2, 4, 3 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 1, 3, 2, 4 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 1, 4, 3, 2 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 2, 1, 3, 4 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 2, 3, 4, 1 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 2, 4, 1, 3 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 3, 1, 4, 2 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 3, 2, 1, 4 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 3, 4, 2, 1 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 4, 1, 2, 3 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 4, 2, 3, 1 }, reference) == -1);
        static_assert(permutation_sign(composition_t<4> { 4, 3, 1, 2 }, reference) == -1);
    }
}