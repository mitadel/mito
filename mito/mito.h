#ifndef __MITO__
#define __MITO__

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <math.h>

#include <pyre/algebra.h>

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
    template <int D>
    using point_t = vector_t<D>;    // Point<D>;
    // templatized typedef for fields
    template <typename X, typename Y>
    using field = Y (*)(const X &, real);
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

// traits on mito types
namespace mito {
    template <typename T>
    struct size {
        static constexpr int value = T::size;
    };

    template <>
    struct size<int> {
        static constexpr int value = 1;
    };

    template <>
    struct size<bool> {
        static constexpr int value = 1;
    };

    template <>
    struct size<double> {
        static constexpr int value = 1;
    };

    template <typename T>
    struct type {
        using value = typename T ::type;
    };

    template <>
    struct type<int> {
        using value = int;
    };

    template <>
    struct type<bool> {
        using value = bool;
    };

    template <>
    struct type<double> {
        using value = double;
    };
}

namespace mito {
    template <typename X, typename Y>
    int AddDirichletBC(std::string boundaryLabel, const field<X, Y> & myF)
    {
        std::cout << "Setting Dirichlet BC on " << boundaryLabel << std::endl;

        X x;
        std::fill(x.begin(), x.end(), 0.0);

        real t = 0.0;

        std::cout << "\tEvaluating BC field at " << x << std::endl;

        auto y = myF(x, t);

        std::cout << "\tmyF(x, t) = " << y << std::endl;

        return 0;
    }
}
#endif    //__MITO__
// end of file
