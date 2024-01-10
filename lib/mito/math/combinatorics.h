// code guard
#if !defined(mito_math_combinatorics_h)
#define mito_math_combinatorics_h


namespace mito::math {

    // swap-sort an {array} in the same order of a {reference} array and return the number of swaps
    // performed
    // (grab {array} by copy as we damage it in the function)
    template <class T>
    constexpr auto swap_sort(T array, const T & reference)
    {
        // get the first element of {array}
        auto first = std::begin(array);
        // get the last element of {array}
        auto last = std::end(array);
        // get the first element of {reference}
        auto first_reference = std::begin(reference);

        // counter for the number of swaps
        int nswaps = 0;

        // loop on the {array}, excluding the last element
        while (first != last - 1) {
            // find the {first_reference} entry in the range {first} to {last}
            auto ref = std::find(first, last, *first_reference);
            // if the {ref} is not at the place it should be at (based on the order in {reference})
            if (ref != first) {
                // pull the {ref} up front within range {first} to {last}
                std::swap(*first, *ref);
                // increase the swap count
                ++nswaps;
            }
            // move the range of interest of one element to the right
            // (the part of the array up to {first}, included, has already been sorted)
            ++first;
            // same for {reference}
            ++first_reference;
        }

        // return the number of swaps
        return nswaps;
    }

    // check whether the data structures contain the same elements (although possibly in different
    // orders)
    template <class T>
    constexpr auto same_elements(T array1, T array2) -> bool
    {
        // sort the two data structures
        std::sort(array1.begin(), array1.end());
        std::sort(array2.begin(), array2.end());

        // return whether the two data structures are now identical
        return array1 == array2;
    }

    // compute the permutation sign of a data structure {data} with respect to a reference data
    // structure that has the same elements of {data} but sorted in ascending order
    template <class T>
    constexpr auto permutation_sign(const T & data, const T & reference) -> int
    {
        // assert that the two data structures have the same elements
        assert(same_elements(data, reference));

        // if the data structure was sorted from {data} to {reference} in an even number of swaps
        if (swap_sort(data, reference) % 2 == 0) {
            // then the permutation sign is positive
            return +1;
        }

        // otherwise the permutation sign is negative
        return -1;
    }
}


#endif

// end of file
