#include "../../base.h"
#include "../../math.h"

int
main()
{
    // a scalar function
    auto f = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::scalar_t { return cos(x[0] * x[1]); });
    // auto Df = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
    //     return { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[0] };
    // });

    // df/dx[0]
    auto Dfx = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::real { return -sin(x[0] * x[1]) * x[1]; });

    // df/dx[1]
    auto Dfy = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::real { return -sin(x[0] * x[1]) * x[0]; });

    // TOFIX
    // auto Df = mito::math::function<mito::vector_t<2>, 2>({ Dfx, Dfy });
    auto Df = mito::math::Function<mito::vector_t<2>, mito::vector_t<2>>({ Dfx, Dfy });
    // auto Df = mito::math::function(Dfx, Dfy);
    std::cout << Dfx({ 0.5, 0.5 }) << std::endl;
    std::cout << Dfy({ 0.5, 0.5 }) << std::endl;
    std::cout << Df({ 0.5, 0.5 }) << std::endl;

    // nice to have: derivatives come from a vector-valued function
    //      possibility to convert an array of scalar-valued functions to a vector-valued function
    // so we can either write
    // auto f_cosine = mito::math::scalar_field(f, { Dfx, Dfy });
    // or
    // auto f_cosine = mito::math::scalar_field(f, Df);

    return 0;
}