#ifndef __MITO__
#define __MITO__

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>

//https://stackoverflow.com/questions/4295432/typedef-function-pointer
//https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

namespace mito {
    // enum for 1D, 2D or 2D
    enum DIM {DIM1 = 1, DIM2 = 2, DIM3 = 3};
    // typedef for scalars
    typedef double real;
    // typedef for vectors
    template <size_t D>
    using vector = std::array<real, D>;
    // typedef for tensors
    template <size_t D1, size_t D2 = D1>
    using tensor = std::array<real, D1 * D2>;
    // templatized typedef for fields
    template <typename X, typename Y>
    using field = Y(*)(const X&, real);
    // templatized typedef for functions
    template <typename X, typename Y>
    using function = Y(*)(const X&);
}

// overload operator<< for vectors and tensors
std::ostream& operator<<(std::ostream& os, const mito::vector<3>& x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::vector<2>& x)
{
    os << "(" << x[0] << ", " << x[1] << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::tensor<3>& x)
{
    os << "(" 
        << x[0] << ", " << x[1] << ", " << x[2] << "; "
        << x[3] << ", " << x[4] << ", " << x[5] << "; "
        << x[6] << ", " << x[7] << ", " << x[8] 
        << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::tensor<2>& x)
{
    os << "(" 
        << x[0] << ", " << x[1] << "; "
        << x[2] << ", " << x[3] 
        << ")";
    return os;
}

template<typename X>
std::ostream& operator<<(std::ostream& os, const std::vector<X>& x){
    
    if (x.size() == 0) {
        os << "[]";
        return os;
    }

    os << "[" << x[0];

    for (size_t i = 1; i < x.size(); ++i)
    {
        os << ", " << x[i];
    }

    os << "]";

    return os;
}

namespace mito {

    real ComputeDeterminant(const tensor<3>& A)
    {
        return A[0] * (A[4] * A[8] - A[5] * A[7]) - A[1] * (A[3] * A[8] - A[5] * A[6]) +
               A[2] * (A[3] * A[7] - A[4] * A[6]);
    }

    real ComputeDeterminant(const tensor<2>& A)
    {
        return A[0] * A[3] - A[1] * A[2];
    }

    real ComputeInverse(const tensor<3>& A, tensor<3>& invA)
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

    real ComputeInverse(const tensor<2>& A, tensor<2>& invA)
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

    std::cout << "\tmyF(x, t) = " << myF(x, t) << std::endl;

    return 0;
}
}
#endif //__MITO__
// end of file
