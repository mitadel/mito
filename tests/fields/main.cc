#include "../../base.h"
#include "../../math.h"

using mito::vector_t;
using mito::real;

int
main()
{
    // a scalar function
    auto f = mito::math::function([](const vector_t<2> & x) -> real { return cos(x[0] * x[1]); });

    auto Df = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[0] };
    });

    // df/dx[0]
    auto Dfx = mito::math::function(
        [](const vector_t<2> & x) -> real { return -sin(x[0] * x[1]) * x[1]; });

    // df/dx[1]
    auto Dfy = mito::math::function(
        [](const vector_t<2> & x) -> real { return -sin(x[0] * x[1]) * x[0]; });

    // instantiate a scalar field
    auto f_cosine = mito::math::field(f, Df);
    // auto f_cosine = mito::math::field(f, { Dfx, Dfy });

    auto cosine_sum1 = mito::math::field(f + f);
    auto cosine_sum2 = mito::math::field(f_cosine + f_cosine);

    // a vector function
    auto g = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });

    // dg/dx[0]
    auto Dgx = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[1] };
    });

    // dg/dx[1]
    auto Dgy = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { -sin(x[0] * x[1]) * x[0], -sin(x[0] * x[1]) * x[0] };
    });

    // instantiate a vector field
    auto cosineVector = mito::math::field(g, { Dgx, Dgy });

    // a point in the reference configuration
    vector_t<2> x = { 0.0, 0.0 };

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << x << " : " << f_cosine(x)
              << std::endl;
    std::cout << "Evaluating gradient of cosine function at X = " << x << " : "
              << mito::math::grad(f_cosine, x) << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at x = " << x << " : "
              << mito::math::div(cosineVector, x) << std::endl;

    auto gradient = mito::math::grad(f_cosine);
    // evaluate divergence
    auto divergence = mito::math::div(cosineVector);

    std::cout << "Evaluating gradient of cosine function at x = " << x << " : " << gradient(x)
              << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at X = " << x << " : "
              << divergence(x) << std::endl;

    // all done
    return 0;
}