#include "../../mito.h"

template <typename T>
struct size;

template <typename T, int N>
struct size<mito::vector<N, T>> {
    static constexpr int value = N;
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

template <typename T>
struct type {
    using value = typename T ::value_type;
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


template <typename Y>
class Test {

  public:
    Test()
    {
        std::cout << "size = " << size<Y>::value << std::endl;
        // static_assert(std::is_same<mito::real, mito::real>::value);
        // static_assert(std::is_same<mito::real, std::vector<mito::real>::value_type>::value);
        // static_assert(std::is_same<mito::real, typename type<Y>::value>::value);
        if constexpr (std::is_same<mito::real, typename type<Y>::value>::value) {
            std::cout << "Is real! " << std::endl;
        } else if constexpr (std::is_same<bool, typename type<Y>::value>::value) {
            std::cout << "Is bool! " << std::endl;
        } else if constexpr (std::is_same<int, typename type<Y>::value>::value) {
            std::cout << "Is int! " << std::endl;
        }

        typename type<Y>::value c = 0.0;

        // all done
        return;
    }
};

int
main()
{
    // size = 2, type = real
    Test<mito::vector<2>> test1;
    // size = 1, type = real
    Test<mito::real> test2;
    // size = 1, type = int
    Test<int> test3;
    // size = 4, type = int
    Test<mito::vector<4, int>> test4;
    // size = 4, type = double
    Test<mito::vector<4, double>> test5;

    // all done
    return 0;
}