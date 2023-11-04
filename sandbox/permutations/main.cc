#include <iostream>
#include <array>
#include <algorithm>


template <int N>
using composition_t = std::array<int, N>;


template <int N>
inline auto
sort(const composition_t<N> & composition)
{
    // std::cout << "sort" << std::endl;

    // get the oriented simplices from the shared pointers
    auto composition_copy = composition_t<N>(composition);

    auto first_simplex = std::begin(composition_copy);
    auto last_simplex = std::end(composition_copy);

    int nswaps = 0;

    while (first_simplex != last_simplex - 1) {
        auto min_simplex = std::min_element(first_simplex, last_simplex);
        // std::cout << "first " << *first_simplex << std::endl;
        // std::cout << "min " << *min_simplex << std::endl;
        if (first_simplex != min_simplex) {
            std::swap(*first_simplex, *min_simplex);
            ++nswaps;
        }
        // std::cout << composition_copy[0] << " " << composition_copy[1] << " " <<
        // composition_copy[2]
        //           << " " << composition_copy[3] << std::endl;
        ++first_simplex;
    }

    // std::cout << nswaps << std::endl;

    // return the number of swaps
    return nswaps;
}

// compute the orientation of the {composition} with respect to the orientation of {simplex}
template <int N>
inline auto
orientation(const composition_t<N> & composition, const composition_t<N> & reference) -> int
{
    if (sort<N>(composition) % 2 == 0) {
        return +1;
    }
    return -1;
}

int
main()
{
    {
        // reference permutation
        auto reference = composition_t<3> { 1, 2, 3 };
        std::cout << "Reference " << reference[0] << " " << reference[1] << " " << reference[2]
                  << std::endl;

        // odd permutation
        {
            auto array = composition_t<3> { 1, 3, 2 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << std::endl;
            std::cout << orientation<3>(array, reference) << std::endl;
        }

        // even permutation
        {
            auto array = composition_t<3> { 3, 1, 2 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << std::endl;
            std::cout << orientation<3>(array, reference) << std::endl;
        }

        // odd permutation
        {
            auto array = composition_t<3> { 2, 1, 3 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << std::endl;
            std::cout << orientation<3>(array, reference) << std::endl;
        }

        // even permutation
        {
            auto array = composition_t<3> { 2, 3, 1 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << std::endl;
            std::cout << orientation<3>(array, reference) << std::endl;
        }

        // odd permutation
        {
            auto array = composition_t<3> { 3, 2, 1 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << std::endl;
            std::cout << orientation<3>(array, reference) << std::endl;
        }
    }

    {
        // reference permutation
        auto reference = composition_t<4> { 1, 2, 3, 4 };
        std::cout << "Reference " << reference[0] << " " << reference[1] << " " << reference[2]
                  << " " << reference[3] << std::endl;

        // odd permutation
        {
            auto array = composition_t<4> { 1, 3, 2, 4 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                      << std::endl;
            std::cout << orientation<4>(array, reference) << std::endl;
        }

        // even permutation
        {
            auto array = composition_t<4> { 1, 3, 4, 2 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                      << std::endl;
            std::cout << orientation<4>(array, reference) << std::endl;
        }

        // odd permutation
        {
            auto array = composition_t<4> { 3, 1, 4, 2 };
            std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3]
                      << std::endl;
            std::cout << orientation<4>(array, reference) << std::endl;
        }
    }

    return 0;
}