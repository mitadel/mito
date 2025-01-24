// -*- coding: utf-8 -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved
//


// support
#include <cassert>
#include <iostream>
// get the tensor module
#include <pyre/tensor.h>


// use namespace for readability
using namespace pyre::tensor;

// constexpr for loop calling function f(i) for each i in (Start, End)
template <int End, int Index1 = 0, class F>
constexpr void
constexpr_for_1(F && f)
{
    if constexpr (Index1 < End) {
        f.template operator()<Index1>();
        return constexpr_for_1<End, Index1 + 1>(f);
    }
}

// take f(i, j) and return f_I(j) := f(I, j) for I fixed
template <int I, class F>
constexpr auto
f_I(F && f)
{
    return [f]<int j>() {
        f.template operator()<I, j>();
    };
}

// constexpr for loop calling function f(i, j) for each i,j in (Start, End)x(Start, End)
template <int End1, int End2, int Index1 = 0, class F>
constexpr void
_constexpr_for_2(F && f)
{
    if constexpr (Index1 < End1) {
        constexpr_for_1<End2>(f_I<Index1>(f));
        _constexpr_for_2<End1, End2, Index1 + 1>(f);
    }

    // the above expands to the following for Start=0, End=3:
    // constexpr_for_1<Start, End>(f_I<0>(f));
    // constexpr_for_1<Start, End>(f_I<1>(f));
    // constexpr_for_1<Start, End>(f_I<2>(f));
}

template <int End1, int End2, class F>
constexpr void
constexpr_for_2(F && f)
{
    _constexpr_for_2<End1, End2, 0>(f);
}

// take f(i, j, k) and return f_IJ(k) := f(I, J, k) for I, J fixed
template <int I, class F>
constexpr auto
f_IJ(F && f)
{
    return [f]<int j, int k>() {
        f.template operator()<I, j, k>();
    };
}

// constexpr for loop calling function f(i, j, j) for each i,j,k in (Start, End)x(Start, End)
template <int End1, int End2, int End3, int Index1 = 0, int Index2 = 0, class F>
constexpr void
_constexpr_for_3(F && f)
{
    if constexpr (Index1 < End1) {
        constexpr_for_2<End2, End3>(f_IJ<Index1>(f));
        _constexpr_for_3<End1, End2, End3, Index1 + 1, Index2>(f);
    }
}

template <int End1, int End2, int End3, class F>
constexpr void
constexpr_for_3(F && f)
{
    _constexpr_for_3<End1, End2, End3, 0, 0>(f);
}

// take f(i, j, k, l) and return f_IJK(l) := f(I, J, K, l) for I, J, K fixed
template <int I, class F>
constexpr auto
f_IJK(F && f)
{
    return [f]<int j, int k, int l>() {
        f.template operator()<I, j, k, l>();
    };
}

// constexpr for loop calling function f(i, j, k, l) for each i,j,k,l in (Start, End)x(Start,
// End)
template <
    int End1, int End2, int End3, int End4, int Index1 = 0, int Index2 = 0, int Index3 = 0, class F>
constexpr void
_constexpr_for_4(F && f)
{
    if constexpr (Index1 < End1) {
        constexpr_for_3<End2, End3, End4>(f_IJK<Index1>(f));
        _constexpr_for_4<End1, End2, End3, End4, Index1 + 1, Index2, Index3>(f);
    }
}

template <int End1, int End2, int End3, int End4, class F>
constexpr void
constexpr_for_4(F && f)
{
    _constexpr_for_4<End1, End2, End3, End4, 0, 0>(f);
}

// main program
int
main(int argc, char * argv[])
{
    // a vector
    constexpr vector_t<3> x{ 1, 1, 1 };

    // assert all components of the vector are equal to 1
    constexpr_for_1<vector_t<3>::size>([x]<int i>() {
        // std::cout << i << std::endl;
        static_assert(x[i] == 1);
    });

    // a matrix
    constexpr matrix_t<3, 3> A{ 2, 2, 2, 2, 2, 2, 2, 2, 2 };

    // assert all components of the matrix are equal to 2
    constexpr_for_2<3, 3>([A]<int i, int j>() {
        // std::cout << i << "\t" << j << std::endl;
        static_assert(A[{ i, j }] == 2);
    });

    // a symmetric matrix
    constexpr matrix_t<3, 3> A_sym{ 1, 2, 3, 2, 1, 2, 3, 2, 1 };

    // assert all components of the matrix are equal to 2
    constexpr_for_2<3, 3>([A_sym]<int i, int j>() {
        // std::cout << i << "\t" << j << std::endl;
        static_assert(A_sym[{ i, j }] == pyre::tensor::transpose(A_sym)[{ i, j }]);
    });


    // a symmetric matrix
    constexpr matrix_t<3, 3> B{ 1, 0, 0, 0, 2, 0, 0, 0, 3 };

    constexpr matrix_t<3, 3> BB{ 1, 0, 0, 0, 4, 0, 0, 0, 9 };

    // assert all components of the matrix are equal to 2
    std::cout << "constexpr_for_3" << std::endl;
    constexpr_for_3<3, 1, 2>(
        []<int i, int j, int k>() { std::cout << i << "\t" << j << "\t" << k << std::endl; });

    // assert all components of the matrix are equal to 2
    std::cout << "constexpr_for_4" << std::endl;
    constexpr_for_4<3, 2, 2, 4>([]<int i, int j, int k, int l>() {
        std::cout << i << "\t" << j << "\t" << k << "\t" << l << std::endl;
    });

    // all done
    return 0;
}


// end of file
