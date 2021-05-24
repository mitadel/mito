#ifndef __MITO__
#define __MITO__

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <math.h>

// https://stackoverflow.com/questions/4295432/typedef-function-pointer
// https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

// TOFIX: Consider using tuples when possible as opposed to std::array and std::vector

namespace mito {
    using dim_t = size_t;
    // typedef for scalars
    using real = double;
    // typedef for vectors
    template <dim_t D, class T = real>
    using vector = std::array<T, D>;
    template <dim_t D>
    using point_t = mito::vector<D>;    // Point<D>;
    // typedef for tensors
    template <dim_t D1, dim_t D2 = D1>
    using tensor = std::array<real, D1 * D2>;
    // templatized typedef for fields
    template <typename X, typename Y>
    using field = Y (*)(const X &, real);
}

// TOFIX: Something is wrong with the overloads of operator<<. Sometimes the compiler won't find
//        the proper one and give an error.

// overload operator<< for vectors and tensors
std::ostream &
operator<<(std::ostream & os, const mito::vector<3> & x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::vector<2> & x)
{
    os << "(" << x[0] << ", " << x[1] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::tensor<3> & x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << "; " << x[3] << ", " << x[4] << ", "
       << x[5] << "; " << x[6] << ", " << x[7] << ", " << x[8] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::tensor<2> & x)
{
    os << "(" << x[0] << ", " << x[1] << "; " << x[2] << ", " << x[3] << ")";
    return os;
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

namespace mito {

    template <dim_t D>
    int Factorial()
    {
        return D * Factorial<dim_t(D - 1)>();
    }
    template <>
    int Factorial<1>()
    {
        return 1;
    }

    real ComputeDeterminant(const tensor<4> & A)
    {
        return A[1] * A[11] * A[14] * A[4] - A[1] * A[10] * A[15] * A[4]
             - A[11] * A[13] * A[2] * A[4] + A[10] * A[13] * A[3] * A[4]
             - A[0] * A[11] * A[14] * A[5] + A[0] * A[10] * A[15] * A[5]
             + A[11] * A[12] * A[2] * A[5] - A[10] * A[12] * A[3] * A[5]
             - A[1] * A[11] * A[12] * A[6] + A[0] * A[11] * A[13] * A[6]
             + A[1] * A[10] * A[12] * A[7] - A[0] * A[10] * A[13] * A[7]
             - A[15] * A[2] * A[5] * A[8] + A[14] * A[3] * A[5] * A[8] + A[1] * A[15] * A[6] * A[8]
             - A[13] * A[3] * A[6] * A[8] - A[1] * A[14] * A[7] * A[8] + A[13] * A[2] * A[7] * A[8]
             + A[15] * A[2] * A[4] * A[9] - A[14] * A[3] * A[4] * A[9] - A[0] * A[15] * A[6] * A[9]
             + A[12] * A[3] * A[6] * A[9] + A[0] * A[14] * A[7] * A[9] - A[12] * A[2] * A[7] * A[9];
    }

    real ComputeDeterminant(const tensor<3> & A)
    {
        return A[0] * (A[4] * A[8] - A[5] * A[7]) - A[1] * (A[3] * A[8] - A[5] * A[6])
             + A[2] * (A[3] * A[7] - A[4] * A[6]);
    }

    real ComputeDeterminant(const tensor<2> & A) { return A[0] * A[3] - A[1] * A[2]; }

    real ComputeInverse(const tensor<3> & A, tensor<3> & invA)
    {
        real det = ComputeDeterminant(A);
        assert(det != 0.0);

        real detinv = 1.0 / det;
        invA[0] = detinv * (A[4] * A[8] - A[5] * A[7]);
        invA[1] = detinv * (-A[1] * A[8] + A[2] * A[7]);
        invA[2] = detinv * (A[1] * A[5] - A[2] * A[4]);
        invA[3] = detinv * (-A[3] * A[8] + A[5] * A[6]);
        invA[4] = detinv * (A[0] * A[8] - A[2] * A[6]);
        invA[5] = detinv * (-A[0] * A[5] + A[2] * A[3]);
        invA[6] = detinv * (A[3] * A[7] - A[4] * A[6]);
        invA[7] = detinv * (-A[0] * A[7] + A[1] * A[6]);
        invA[8] = detinv * (A[0] * A[4] - A[1] * A[3]);

        return det;
    }

    real ComputeInverse(const tensor<2> & A, tensor<2> & invA)
    {
        real det = ComputeDeterminant(A);
        assert(det != 0.0);

        real detinv = 1.0 / det;
        invA[0] = detinv * (A[3]);
        invA[1] = detinv * (-A[1]);
        invA[2] = detinv * (-A[2]);
        invA[3] = detinv * (A[0]);

        return det;
    }
}

// traits on mito types
namespace mito {
    template <typename T>
    struct size;

    template <typename T, int N>
    struct size<mito::vector<N, T>> {
        static constexpr int value = N;
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
        using value = typename T ::value_type;
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
