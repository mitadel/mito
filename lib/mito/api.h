// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_api_h)
#define mito_api_h


// https://stackoverflow.com/questions/4295432/typedef-function-pointer
// https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

namespace mito {

    // typedef for real values
    using real = double;
    // typedef for scalars
    using scalar_t = real;
    // typedef for vectors
    template <int D, typename T = real>
    using vector_t = pyre::tensor::vector_t<D, T>;
    // typedef for matrices
    template <int D1, int D2 = D1, typename T = real>
    using matrix_t = pyre::tensor::matrix_t<D1, D2, T>;
    // typedef for symmetric matrices
    template <int D, typename T = real>
    using symmetric_matrix_t = pyre::tensor::symmetric_matrix_t<D, T>;
    // typedef for diagonal matrices
    template <int D, typename T = real>
    using diagonal_matrix_t = pyre::tensor::diagonal_matrix_t<D, T>;

    // concept of a scalar
    template <class F>
    concept scalar_c = std::convertible_to<F, scalar_t>;

    // concept of a vector
    template <class F>
    concept vector_c = requires(F c) {
        // require that F only binds to {vector_t} specializations
        []<int D, typename T>(vector_t<D, T> &) {
        }(c);
    };

    // concept of a tensor
    template <class F>
    concept tensor_c = requires(F c) {
        // require that F only binds to Tensor<T, packingT, I...> specializations
        []<typename T, class packingT, int... I>(pyre::tensor::Tensor<T, packingT, I...> &) {
        }(c);
    };

    // concept for a tensor (vector, matrix, higher-order, ...) or a scalar
    template <class F>
    concept tensor_or_scalar_c = tensor_c<F> or std::convertible_to<F, scalar_t>;

    // concept of a matrix
    template <class F>
    concept matrix_c = tensor_c<F> and requires { F::rank == 2; };

    // concept of a diagonal matrix
    template <class F>
    concept diagonal_matrix_c = matrix_c<F> and requires { F::diagonal; };

    // concept of a symmetric matrix
    template <class F>
    concept symmetric_matrix_c = (matrix_c<F> and requires { F::symmetric; }) or diagonal_matrix_c<F>;

    template <typename X>
    std::ostream & operator<<(std::ostream & os, const std::vector<X> & x)
    {

        if (std::size(x) == 0) {
            os << "[]";
            return os;
        }

        os << "[" << x[0];

        for (auto i = 1; i < std::size(x); ++i) {
            os << ", " << x[i];
        }

        os << "]";

        return os;
    }

    template <typename X, long unsigned int N>
    std::ostream & operator<<(std::ostream & os, const std::array<X, N> & x)
    {

        if (N == 0) {
            os << "[]";
            return os;
        }

        os << "[" << x[0];

        for (long unsigned int i = 1; i < N; ++i) {
            os << ", " << x[i];
        }

        os << "]";

        return os;
    }

    // TOFIX: perhaps move the pyre wrapping to a separate header? This stuff should be under
    //          external

    // I-th basis vector in dimension D
    template <int I, int D>
    requires(D > 0 && I >= 0 && I < D)
    constexpr auto e = pyre::tensor::unit<pyre::tensor::vector_t<D>, I>;

    template <int D>
    constexpr auto e_0 = e<0, D>;

    template <int D>
    constexpr auto e_1 = e<1, D>;

    template <int D>
    constexpr auto e_2 = e<2, D>;

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

    template <int D>
    requires(D > 0)
    constexpr auto identity = pyre::tensor::identity<pyre::tensor::matrix_t<D>>;

    // sequences of integers
    template <int N>
    using make_integer_sequence = pyre::tensor::make_integer_sequence<N>;
    template <int... I>
    using integer_sequence = pyre::tensor::integer_sequence<I...>;
}

#endif

// end of file
