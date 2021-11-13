#include <iostream>
#include <utility>
#include <array>
#include <cassert>

/*
int entries(int N, int D)
{
    if (N == 1) return D;

    int number = 0;
    for (int i = 0; i < D; ++i)
    {
        number += entries(N - 1, D - i);
    }

    return number;
}*/

template<int D> 
int entries(int N)
{
    if (N == 1) return D;

    // helper function (sum all entries for i = 0, ..., D - 1)
    constexpr auto _sum_entries = []<size_t... i>(int N, std::index_sequence<i...>)
    {
        // all done
        return (entries<D-i>(N-1) + ...);
    };

    return _sum_entries(N, std::make_index_sequence<D> {});
}

// template<int D, int N, class... T> 
// int offset(T... args) requires (sizeof...(T) == N)
// {
//     return args[0];
// }

// helper function to compute the product of an arguments list
template <typename... Args>
static constexpr auto sum(Args &&... args) -> int
{
    if constexpr (sizeof...(args))
        return  (args + ...);
    return 0;
}

template<int D, int N, int i> 
int offset1()
{
    static_assert(N==1);
    return i;
}

template<int D, int N, int i, int j> 
int offset2()
{
    static_assert(N==2);
    // helper function (sum all entries for i = 0, ..., D - 1)
    constexpr auto _sum_entries = []<size_t... a>(std::index_sequence<a...>)
    {
        // all done
        return sum(entries<D - a>(N - 1) ...);
    };

    // std::cout << "_sum_entries = " << _sum_entries(std::make_index_sequence<i> {}) << std::endl;
    return _sum_entries(std::make_index_sequence<i> {}) + offset1<D - i, N - 1, j>();
}

template<int D, int N, int i, int j> 
int offset2_wrapped()
{
    static_assert(N==2);
    // sort indices from min to max
    static_assert(i <= j);
    return offset2<D, N, i, j - i>();
}

template<int D, int N, int i, int j, int k> 
int offset3()
{
    static_assert(N==3);
    // helper function (sum all entries for i = 0, ..., D - 1)
    constexpr auto _sum_entries = []<size_t... a>(std::index_sequence<a...>)
    {
        // all done
        return sum(entries<D - a>(N - 1) ...);
    };

    return _sum_entries(std::make_index_sequence<i> {}) + offset2<D - i, N - 1, j, k>();
}

template<int D, int N, int i, int j, int k> 
int offset3_wrapped()
{
    static_assert(N==3);
    // constexpr std::array<int, 3> a = {i, j, k};
    // // sort indices from min to max
    // std::sort(a.begin(), a.end());
    //return offset3<D, N, a[0], a[1] - a[0], a[2] - a[1]>();
    static_assert(i <= j && j <= k);
    return offset3<D, N, i, j - i, k - j>();
}

int main() {

    assert(entries<2 /* D */>(2 /* N */) == 3);
    assert(entries<2 /* D */>(3 /* N */) == 4);
    assert(entries<3 /* D */>(2 /* N */) == 6);
    assert(entries<3 /* D */>(3 /* N */) == 10);

#if 1
    int offset = 0;

    std::cout << "(0, 0) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 0, 0>();
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 0, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 0, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 1, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2) " << std::endl;
    offset = offset2_wrapped<3 /* D */, 2 /* N */, 2, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif

#if 0
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3_wrapped<2 /* D */, 3 /* N */, 0, 0, 0>();
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3_wrapped<2 /* D */, 3 /* N */, 0, 0, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3_wrapped<2 /* D */, 3 /* N */, 0, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3_wrapped<2 /* D */, 3 /* N */, 1, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
#if 0
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 0, 0>();
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 0, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 0, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 1, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 0, 2, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 1, 1, 1>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 1, 1, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 1, 2, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2, 2) " << std::endl;
    offset = offset3_wrapped<3 /* D */, 3 /* N */, 2, 2, 2>();
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
    return 0;
}