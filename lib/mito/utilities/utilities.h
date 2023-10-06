// code guard
#if !defined(mito_utilities_utilities_h)
#define mito_utilities_utilities_h


namespace mito::utilities {

    // hash function for shared pointers
    // Note that two pointers pointing to the same cell collapse on the same hashed value
    template <class sharedPointerT>
    struct hash_function {
        size_t operator()(const sharedPointerT & item) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<size_t>(item.id());
        }
    };

    // strip constant/volatile and reference from type
    template <typename T>
    using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
}


#endif

// end of file
