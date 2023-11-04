// code guard
#if !defined(mito_math_combinatorics_h)
#define mito_math_combinatorics_h


namespace mito::math {

    template <class T>
    inline auto sort(T array)
    {
        // get the first element
        auto first = std::begin(array);
        // get the last element
        auto last = std::end(array);

        // counter for the number of swaps
        int nswaps = 0;

        // loop on the {array}, excluding the last element
        while (first != last - 1) {
            // get the minimum entry in the range {first} to {last}
            auto min = std::min_element(first, last);
            // if the {min} is not at the place it be at (based on ascending order)
            if (min != first) {
                // put the {min} up front
                std::swap(*first, *min);
                // increase the swap count
                ++nswaps;
            }
            // move the range of interest of one element to the right
            // (the part of the array up to {first}, included, has already been sorted)
            ++first;
        }

        // return the number of swaps
        return nswaps;
    }

    // compute the permutation sign of a data structure {data} with respect to a reference data
    // structure that has the same elements of {data} but sorted in ascending order
    template <class T>
    inline auto permutation_sign(const T & data) -> int
    {
        // if the data structure was sorted in an even number of swaps
        if (sort(data) % 2 == 0) {
            // then the permutation sign is positive
            return +1;
        }

        // otherwise the permutation sign is negative
        return -1;
    }
}


#endif

// end of file
