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
    using vector_t = pyre::algebra::vector_t<D, T>;
    // typedef for matrices
    template <int D1, int D2 = D1, typename T = real>
    using matrix_t = pyre::algebra::matrix_t<D1, D2, T>;
    // typedef for symmetric matrices
    template <int D, typename T = real>
    using symmetric_matrix_t = pyre::algebra::symmetric_matrix_t<D, T>;
    // typedef for diagonal matrices
    template <int D, typename T = real>
    using diagonal_matrix_t = pyre::algebra::diagonal_matrix_t<D, T>;

}

template <typename X>
std::ostream &
operator<<(std::ostream & os, const std::vector<X> & x)
{

    if (x.size() == 0) {
        os << "[]";
        return os;
    }

    os << "[" << x[0];

    for (auto i = 1; i < x.size(); ++i) {
        os << ", " << x[i];
    }

    os << "]";

    return os;
}

template <typename X, long unsigned int N>
std::ostream &
operator<<(std::ostream & os, const std::array<X, N> & x)
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


#endif

// end of file
