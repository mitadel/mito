#include "../../mito.h"

template <typename Y>
class Test {

  public:
    Test()
    {
        std::cout << "size = " << mito::size<Y>::value << std::endl;
        // static_assert(std::is_same<mito::real, mito::real>::value);
        // static_assert(std::is_same<mito::real, std::vector<mito::real>::value_type>::value);
        // static_assert(std::is_same<mito::real, typename type<Y>::value>::value);
        if constexpr (std::is_same<mito::real, typename mito::type<Y>::value>::value) {
            std::cout << "Is real! " << std::endl;
        } else if constexpr (std::is_same<bool, typename mito::type<Y>::value>::value) {
            std::cout << "Is bool! " << std::endl;
        } else if constexpr (std::is_same<int, typename mito::type<Y>::value>::value) {
            std::cout << "Is int! " << std::endl;
        }

        typename mito::type<Y>::value c = 0.0;

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