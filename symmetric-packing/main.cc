#include <iostream>
#include <utility>
#include <array>
#include <cassert>
#include <tuple>

template<int N>
constexpr int entries(int D) requires (N == 1)
{
    return D;
}

template<int N>
constexpr int entries(int D) requires (N > 1)
{
    int sum_entries = 0;
    for (int i = 0; i < D; ++i)
    {
        sum_entries += entries<N - 1>(D - i);
    }

    return sum_entries;

}

template<int N, class... T> 
constexpr int offset(int D, int i) requires (sizeof...(T) == N - 1 && N == 1)
{
    return i;
}

template<int N, class... T> 
constexpr int offset(int D, int i, T... j) requires (sizeof...(T) == N - 1 && N > 1)
{
    int sum_entries = 0;
    for (int a = 0; a < i; ++a)
    {
        sum_entries += entries<N - 1>(D - a);
    }

    return sum_entries + offset<N - 1>(D - i, j...);
}

template<int D, int N, class... T> 
constexpr int offsetN(T... index) requires (sizeof...(T) == N)
{
    // copy the index sequence into an array
    std::array<int, N> shiftedIndex {index...};    
    std::sort(shiftedIndex.begin(), shiftedIndex.end());

    // shift the indices (i, j, k, ...) into (i, j - i, k - j, ...)
    auto _shiftIndices = []<size_t... I>(std::array<int, N> & i, std::index_sequence<I...>)
    {
        // iterate in reverse order to avoid overwriting entries before using them
        ((i[(N - 2 - I) + 1] -= i[N - 2 - I]), ... );
        // all done
        return;
    };

    // shift the indices (i, j, k, ...) into (i, j - i, k - j, ...)
    _shiftIndices(shiftedIndex, std::make_index_sequence<N-1> {});

    // helper function needed to expand the tuple in a parameter pack
    auto _getOffset = []<size_t... I>(std::array<int, N> i, std::index_sequence<I...>)
    {
        return offset<N>(D, i[I]...);
    };

    // get the offset
    return _getOffset(shiftedIndex, std::make_index_sequence<N> {});
}

template<int D, int N> 
int offset2(int i, int j)
{
    static_assert(N == 2);
    // assert indices are sorted
    assert(i <= j);
    return offset<N>(D, i, j - i);
}

template<int D, int N> 
int offset3(int i, int j, int k)
{
    static_assert(N==3);
    // assert indices are sorted
    assert(i <= j && j <= k);
    return offset<N>(D, i, j - i, k - j);
}

int main() {

    static_assert(entries<2 /* N */>(2 /* D */) == 3);
    static_assert(entries<3 /* N */>(2 /* D */) == 4);
    static_assert(entries<2 /* N */>(3 /* D */) == 6);
    static_assert(entries<3 /* N */>(3 /* D */) == 10);

#if 0
    int offset = 0;

    std::cout << "(0, 0) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(0, 0);
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(0, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(0, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(1, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2) " << std::endl;
    offset = offset2<3 /* D */, 2 /* N */>(2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif

#if 0
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3<2 /* D */, 3 /* N */>(0, 0, 0);
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3<2 /* D */, 3 /* N */>(0, 0, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3<2 /* D */, 3 /* N */>(0, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3<2 /* D */, 3 /* N */>(1, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
#if 1
    int offset = 0;

    std::cout << "(0, 0, 0) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */> (0, 0, 0);
    std::cout << "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 1) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(0, 0, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 0, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(0, 0, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 1) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(0, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 1, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(0, 1, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(0, 2, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(0, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 1) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(1, 1, 1);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 1, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(1, 1, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(1, 2, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(1, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;

    std::cout << "(2, 2, 2) " << std::endl;
    offset = offset3<3 /* D */, 3 /* N */>(2, 2, 2);
    std::cout <<  "offset " << offset << std::endl;
    std::cout << std::endl;
#endif
    return 0;
}