#include "../../math/Function.h"
#include <cmath>

using mito::Function;

static const mito::real PI = 4.0 * atan(1.0);
static const mito::real TOL = 1.e-16;

// TOFIX: Should we also account for real(const X &) + Function<X,
// scalar_t> ?
//
// mito::real
mito::scalar_t
my_function(const mito::vector_t<2> & x)
{
    return cos(x[0] * x[1]) + 1.0;
}

int
main()
{
    // a scalar function
    Function<mito::vector_t<2>> function1(
        [](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });

    // a scalar function
    Function<mito::vector_t<2>> function2([](const mito::vector_t<2> & x) { return 5; });

    // the sum of the two scalar functions
    Function<mito::vector_t<2>> function3 = function1 + function2;

    // a point in the reference configuration
    mito::vector_t<2> x = { 1.0, PI };

    std::cout << "x = " << x << std::endl;
    std::cout << "f1 = cos(x[0]*x[1]), f1(x) = " << function1(x) << std::endl;
    std::cout << "f2 = 5 , f2(x) = " << function2(x) << std::endl;
    std::cout << "f1(x) + f2(x) = " << function3(x) << std::endl;

    auto function4 = function3 + function3 + function3;
    auto function5 = 3.0 * function3;
    assert(std::fabs(function4(x) - function5(x)) < TOL);

    auto function6 = function4 - function3;
    auto function7 = function5 - function3;
    assert(std::fabs(function6(x) - function7(x)) < TOL);

    auto function8 = (function7 * 0.5) / 0.5;
    assert(std::fabs(function7(x) - function8(x)) < TOL);

    auto function9 = (0.5 / function7) * function7;
    assert(std::fabs(function9(x) - 0.5) < TOL);

    auto function10 = (PI + function7) - function7;
    assert(std::fabs(function10(x) - PI) < TOL);

    auto function11 = (PI + function7) - (2.0 * PI);
    assert(std::fabs(function11(x) - function7(x) + PI) < TOL);

    // (cos(xy) + 5) / cos(xy)
    auto function12 = (function1 + function2) / function1;
    assert(std::fabs((function1(x) + function2(x)) / function1(x) - function12(x)) < TOL);

    Function<mito::vector_t<2>> function13(my_function);
    auto function14 = function13 + function1;

    // TOFIX: Should we also account for std::function<real(const X &)> + Function<X,
    // scalar_t> ?
    //
    // std::function<mito::real(const mito::vector_t<2> &)> my_other_function(my_function);
    std::function<mito::scalar_t(const mito::vector_t<2> &)> my_other_function(
        my_function);
    auto function15 = my_other_function + function1 + my_function;

    // a vector function
    Function<mito::vector_t<2>, mito::vector_t<3>> function16([](const mito::vector_t<2> & x) {
        return mito::vector_t<3> { cos(x[0] * x[1]), cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });

    // vector times scalar multiplication
    mito::real alpha = 10;
    auto function17 = alpha * function16;
    std::cout << "function16 = " << function16(x) << std::endl;
    std::cout << "function17 = " << function17(x) << std::endl;

    // inner product between vectors
    mito::vector_t<3> my_vector = { 1, 2, 3 };
    auto function18 = my_vector * function16;
    std::cout << "function16 = " << function16(x) << std::endl;
    std::cout << "function17 = " << function18(x) << std::endl;

    return 0;
}