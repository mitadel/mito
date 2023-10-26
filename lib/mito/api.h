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
    requires(D > 0 && I > 0 && I < D)
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
    requires(D > 1)
    constexpr auto identity = pyre::tensor::identity<pyre::tensor::matrix_t<D>>;
}

#endif

// end of file
