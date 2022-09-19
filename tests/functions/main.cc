#include <mito/base.h>
#include <mito/math.h>

using mito::math::function_t;

static const mito::real PI = 4.0 * atan(1.0);
static const mito::real TOL = 1.e-16;

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
    auto function1 = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::scalar_t { return cos(x[0] * x[1]); });

    // a scalar function
    auto function2 =
        mito::math::function([](const mito::vector_t<2> & /*x*/) -> mito::scalar_t { return 5; });

    // the sum of the two scalar functions
    auto function3 = function1 + function2;

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

    auto function11 = PI + function7 - 2.0 * PI;
    assert(std::fabs(function11(x) - function7(x) + PI) < TOL);

    // (cos(xy) + 5) / cos(xy)
    auto function12 = (function1 + function2) / function1;
    assert(std::fabs((function1(x) + function2(x)) / function1(x) - function12(x)) < TOL);

    auto function13 = mito::math::function(my_function);
    auto function14 = function13 + function1;

    std::function<mito::real(const mito::vector_t<2> &)> my_other_f(my_function);
    auto my_other_function = mito::math::function(my_other_f);
    auto function15 = my_other_function + function1;

    // a vector function
    auto function16 = mito::math::function([](const mito::vector_t<2> & x) -> mito::vector_t<3> {
        return { cos(x[0] * x[1]), cos(x[0] * x[1]), cos(x[0] * x[1]) };
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
    std::cout << "function18 = " << function18(x) << std::endl;

    return 0;
}