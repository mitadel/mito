#include "../../math/function.h"

using mito::Function;

int
main()
{
    // a scalar function
    Function<mito::vector<2>, mito::real> function1(
        [](const mito::vector<2> & x) { return cos(x[0] * x[1]); });

    // a scalar function
    Function<mito::vector<2>, mito::real> function2([](const mito::vector<2> & x) { return 5; });

    // the sum of the two scalar functions
    Function<mito::vector<2>, mito::real> function3 = function1 + function2;

    // a point in the reference configuration
    mito::vector<2> x = { 0.0, 0.0 };

    std::cout << function1(x) << std::endl;
    std::cout << function2(x) << std::endl;
    std::cout << function3(x) << std::endl;

    auto function4 = function1 + function2 + function3;
    std::cout << function4(x) << std::endl;

    return 0;
}