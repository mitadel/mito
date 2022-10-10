#include <gtest/gtest.h>
#include <mito/base.h>

TEST(Algebra, TestAlgebra)
{
    /*constexpr*/ mito::matrix_t<3> Du { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 1.0 };

    int i = 0;
    int N = 10000000;
    while (i < N) {
#if 1
        /*constexpr*/ mito::matrix_t<3> A = Du + Du;
#else
        mito::matrix_t<3> A;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // for (int k = 0; k < 3; k++) {
                A[i * 3 + j] = Du[i * 3 + j] + Du[i * 3 + j];
                //}
            }
        }
#endif
        ++i;
        // std::cout << A << std::endl;
    }
}