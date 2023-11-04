#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


template <int N>
using composition_t = std::array<int, N>;


TEST(Math, PermutationSign)
{
    // make a channel
    pyre::journal::info_t channel("mito.math.permutation_sign");

    {
        // reference permutation 3D
        auto reference = composition_t<3> { 1, 2, 3 };
        channel << "Reference " << reference[0] << " " << reference[1] << " " << reference[2]
                << pyre::journal::endl;

        // odd permutation
        {
            auto array = composition_t<3> { 1, 3, 2 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, -1);
            channel << array[0] << " " << array[1] << " " << array[2] << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // even permutation
        {
            auto array = composition_t<3> { 3, 1, 2 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, +1);
            channel << array[0] << " " << array[1] << " " << array[2] << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // odd permutation
        {
            auto array = composition_t<3> { 2, 1, 3 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, -1);
            channel << array[0] << " " << array[1] << " " << array[2] << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // even permutation
        {
            auto array = composition_t<3> { 2, 3, 1 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, +1);
            channel << array[0] << " " << array[1] << " " << array[2] << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // odd permutation
        {
            auto array = composition_t<3> { 3, 2, 1 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, -1);
            channel << array[0] << " " << array[1] << " " << array[2] << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }
    }

    {
        // reference permutation 4D
        auto reference = composition_t<4> { 1, 2, 3, 4 };
        channel << "Reference " << reference[0] << " " << reference[1] << " " << reference[2] << " "
                << reference[3] << pyre::journal::endl;

        // odd permutation
        {
            auto array = composition_t<4> { 1, 3, 2, 4 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, -1);
            channel << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                    << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // even permutation
        {
            auto array = composition_t<4> { 1, 3, 4, 2 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, +1);
            channel << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                    << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }

        // odd permutation
        {
            auto array = composition_t<4> { 3, 1, 4, 2 };
            auto sign = mito::math::permutation_sign(array);
            EXPECT_EQ(sign, -1);
            channel << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                    << pyre::journal::endl;
            channel << sign << pyre::journal::endl;
        }
    }
}