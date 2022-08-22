// code guard
#if !defined(mito_traits_h)
#define mito_traits_h


// traits on mito types
namespace mito {
    template <class T>
    struct size {
        static constexpr int value = T::size;
    };

    template <>
    struct size<int> {
        static constexpr int value = 1;
    };

    template <>
    struct size<bool> {
        static constexpr int value = 1;
    };

    template <>
    struct size<double> {
        static constexpr int value = 1;
    };

    template <class T>
    struct type {
        using value = typename T ::type;
    };

    template <>
    struct type<int> {
        using value = int;
    };

    template <>
    struct type<bool> {
        using value = bool;
    };

    template <>
    struct type<double> {
        using value = double;
    };
}


#endif

// end of file
