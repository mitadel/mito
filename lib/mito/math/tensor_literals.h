// code guard
#if !defined(mito_math_tensor_literals_h)
#define mito_math_tensor_literals_h


// the most commonly used packing-agnostic canonical basis
namespace mito::math {

    template <int D>
    requires(D > 0)
    constexpr auto e_0 = pyre::tensor::unit<pyre::tensor::vector_t<D>, 0>;

    template <int D>
    requires(D > 1)
    constexpr auto e_1 = pyre::tensor::unit<pyre::tensor::vector_t<D>, 1>;

    template <int D>
    requires(D > 2)
    constexpr auto e_2 = pyre::tensor::unit<pyre::tensor::vector_t<D>, 2>;

    template <int D>
    requires(D > 0)
    constexpr auto e_00 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 0, 0>;

    template <int D>
    requires(D > 1)
    constexpr auto e_01 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 0, 1>;

    template <int D>
    requires(D > 1)
    constexpr auto e_10 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 1, 0>;

    template <int D>
    requires(D > 1)
    constexpr auto e_11 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 1, 1>;

    template <int D>
    requires(D > 2)
    constexpr auto e_02 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 0, 2>;

    template <int D>
    requires(D > 2)
    constexpr auto e_20 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 2, 0>;

    template <int D>
    requires(D > 2)
    constexpr auto e_12 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 1, 2>;

    template <int D>
    requires(D > 2)
    constexpr auto e_21 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 2, 1>;

    template <int D>
    requires(D > 2)
    constexpr auto e_22 = pyre::tensor::unit<pyre::tensor::matrix_t<D>, 2, 2>;

}


#endif

// end of file
