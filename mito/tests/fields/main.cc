#include "../../base.h"
#include "../../math.h"

int
main()
{
    // a scalar function
    auto f = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::scalar_t { return cos(x[0] * x[1]); });
    auto Df_a = mito::math::function([](const mito::vector_t<2> & x) -> mito::vector_t<2> {
        return { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[0] };
    });

    // df/dx[0]
    auto Dfx = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::real { return -sin(x[0] * x[1]) * x[1]; });

    // df/dx[1]
    auto Dfy = mito::math::function(
        [](const mito::vector_t<2> & x) -> mito::real { return -sin(x[0] * x[1]) * x[0]; });

    auto Df_b = mito::math::function_t<mito::vector_t<2>, mito::vector_t<2>>({ Dfx, Dfy });
    auto Df_c = mito::math::function({ Dfx, Dfy });
    std::cout << Dfx({ 0.5, 0.5 }) << std::endl;
    std::cout << Dfy({ 0.5, 0.5 }) << std::endl;
    std::cout << Df_a({ 0.5, 0.5 }) << std::endl;
    std::cout << Df_b({ 0.5, 0.5 }) << std::endl;
    std::cout << Df_c({ 0.5, 0.5 }) << std::endl;

    auto field_a = mito::math::scalar_field(f, Df_a);
    auto field_b = mito::math::scalar_field(f, Df_b);
    auto field_c = mito::math::scalar_field(f, Df_c);
    auto field   = mito::math::scalar_field(f, { Dfx, Dfy });
    // or
    // auto f_cosine = mito::math::scalar_field(f, Df);

    return 0;
}