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
template <int Start, int End, class F>
constexpr void
constexpr_for(F && f)
{
    if constexpr (Start < End) {
        f.template operator()<Start>();
        return constexpr_for<Start + 1, End>(f);
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
template <int Start1, int End1, int Start2, int End2, int Index1 = Start1, class F>
constexpr void
constexpr_for_tensor(F && f)
{
    if constexpr (Index1 < End1) {
        constexpr_for<Start2, End2>(f_I<Index1>(f));
        constexpr_for_tensor<Start1, End1, Start2, End2, Index1 + 1>(f);
    }

    // the above expands to the following for Start=0, End=3:
    // constexpr_for<Start, End>(f_I<0>(f));
    // constexpr_for<Start, End>(f_I<1>(f));
    // constexpr_for<Start, End>(f_I<2>(f));
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
template <
    int Start1, int End1, int Start2, int End2, int Start3, int End3, int Index1 = Start1,
    int Index2 = Start2, class F>
constexpr void
constexpr_for_3(F && f)
{
    if constexpr (Index1 < End1) {
        constexpr_for_tensor<Start2, End2, Start3, End3>(f_IJ<Index1>(f));
        constexpr_for_3<Start1, End1, Start2, End2, Start3, End3, Index1 + 1, Index2>(f);
    }
}

// main program
int
main(int argc, char * argv[])
{

    // a vector
    constexpr vector_t<3> x { 1, 1, 1 };

    // assert all components of the vector are equal to 1
    constexpr_for<0, vector_t<3>::size>([x]<int i>() {
        // std::cout << i << std::endl;
        static_assert(x[i] == 1);
    });

    // a matrix
    constexpr matrix_t<3, 3> A { 2, 2, 2, 2, 2, 2, 2, 2, 2 };

    // assert all components of the matrix are equal to 2
    constexpr_for_tensor<0, 3, 0, 3>([A]<int i, int j>() {
        // std::cout << i << "\t" << j << std::endl;
        static_assert(A[{ i, j }] == 2);
    });

    // a symmetric matrix
    constexpr matrix_t<3, 3> A_sym { 1, 2, 3, 2, 1, 2, 3, 2, 1 };

    // assert all components of the matrix are equal to 2
    constexpr_for_tensor<0, 3, 0, 3>([A_sym]<int i, int j>() {
        // std::cout << i << "\t" << j << std::endl;
        static_assert(A_sym[{ i, j }] == pyre::tensor::transpose(A_sym)[{ i, j }]);
    });


    // a symmetric matrix
    constexpr matrix_t<3, 3> B { 1, 0, 0, 0, 2, 0, 0, 0, 3 };

    constexpr matrix_t<3, 3> BB { 1, 0, 0, 0, 4, 0, 0, 0, 9 };

    // assert all components of the matrix are equal to 2
    constexpr_for_3<0, 3, 0, 1, 0, 2>(
        []<int i, int j, int k>() { std::cout << i << "\t" << j << "\t" << k << std::endl; });


    // all done
    return 0;
}


// end of file
