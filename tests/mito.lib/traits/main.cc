#include <gtest/gtest.h>
#include <mito/base.h>

template <typename Y>
class ClassTest {

  public:
    ClassTest()
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

        // all done
        return;
    }
};

TEST(Traits, TestTraits)
{
    // size = 2, type = real
    ClassTest<mito::vector_t<2>> test1;
    // size = 1, type = real
    ClassTest<mito::real> test2;
    // size = 1, type = int
    ClassTest<int> test3;
    // size = 4, type = int
    ClassTest<mito::vector_t<4, int>> test4;
    // size = 4, type = double
    ClassTest<mito::vector_t<4, double>> test5;
}