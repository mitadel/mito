#include <iostream>
#include <utility>
#include <array>
#include <cassert>

int entries(int N, int D)
{
    if (N == 1) return D;

    int sum_entries = 0;
    for (int i = 0; i < D; ++i)
    {
        sum_entries += entries(N - 1, D - i);
    }

    return sum_entries;

}

template<int N> 
int offset1(int D, int i)
{
    static_assert(N == 1);
    return i;
}

template<int N> 
int offset2(int D, int i, int j)
{
    static_assert(N == 2);
    int sum_entries = 0;
    for (int a = 0; a < i; ++a)
    {
        sum_entries += entries(N - 1, D - a);
    }

    return sum_entries + offset1<N - 1>(D - i, j);

}

template<int D, int N> 
int offset2_symmetric(int i, int j)
{
    static_assert(N == 2);
    // assert indices are sorted
    assert(i <= j);
    return offset2<N>(D, i, j - i);
}

template<int N> 
int offset3(int D, int i, int j, int k)
{
    static_assert(N == 3);
    int sum_entries = 0;
    for (int a = 0; a < i; ++a)
    {
        sum_entries += entries(N - 1, D - a);
    }

    return sum_entries + offset2<N - 1>(D - i, j, k);
}

template<int D, int N> 
int offset3_symmetric(int i, int j, int k)
{
    static_assert(N==3);
    // assert indices are sorted
    assert(i <= j && j <= k);
    return offset3<N>(D, i, j - i, k - j);
}

int main() {

    assert(entries(2 /* N */, 2 /* D */) == 3);
    assert(entries(3 /* N */, 2 /* D */) == 4);
    assert(entries(2 /* N */, 3 /* D */) == 6);
    assert(entries(3 /* N */, 3 /* D */) == 10);

#if 0
    int offset = 0;

    std::cout << "(0, 0) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 0, 0>();
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 0, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 0, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 1, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2) " << std::endl;
    offset = offset2_symmetric<3 /* D */, 2 /* N */, 2, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif

#if 0
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3_symmetric<2 /* D */, 3 /* N */, 0, 0, 0>();
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3_symmetric<2 /* D */, 3 /* N */, 0, 0, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3_symmetric<2 /* D */, 3 /* N */, 0, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3_symmetric<2 /* D */, 3 /* N */, 1, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
#if 1
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */> (0, 0, 0);
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(0, 0, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(0, 0, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(0, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(0, 1, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(0, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(1, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(1, 1, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(1, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2, 2) " << std::endl;
    offset = offset3_symmetric<3 /* D */, 3 /* N */>(2, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
    return 0;
}