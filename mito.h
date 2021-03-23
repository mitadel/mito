#ifndef __MITO__
#define __MITO__

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>

//https://stackoverflow.com/questions/4295432/typedef-function-pointer
//https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

// TOFIX: Consider using tuples when possible as opposed to std::array and std::vector 

namespace mito {
    // Templating mito::vector and mito::tensor with respect to the enum DIM DOES the trick of 
    // preventing the user from instantiating mito::vector<15>:
    //   "error: invalid conversion from 'int' to 'mito::DIM' [-fpermissive]"

    // enum for 1D, 2D, 3D, 4D (4D is used for parametric coordinates)
    enum DIM {DIM1 = 1, DIM2 = 2, DIM3 = 3, DIM4 = 4};

    static constexpr auto x0 = std::integral_constant<int, 0>{};
    static constexpr auto x1 = std::integral_constant<int, 1>{};
    static constexpr auto x2 = std::integral_constant<int, 2>{};
    static constexpr auto x3 = std::integral_constant<int, 3>{};
 
    // typedef for scalars
    using real = double;
    // typedef for dim loop
    using dim_t = int;
    // typedef for vectors
    template <DIM D>
    using vector = std::array<real, D>;
    // typedef for tensors
    template <DIM D1, DIM D2 = D1>
    using tensor = std::array<real, D1 * D2>;
    // templatized typedef for fields
    template <typename X, typename Y>
    using field = Y(*)(const X&, real);
    // templatized typedef for functions
    template <typename X, typename Y = real>
    using function = Y(*)(const X&);
}

// TOFIX: Something is wrong with the overloads of operator<<. Sometimes the compiler won't find 
//        the proper one and give an error. 

// overload operator<< for vectors and tensors
std::ostream& operator<<(std::ostream& os, const mito::vector<mito::DIM3>& x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::vector<mito::DIM2>& x)
{
    os << "(" << x[0] << ", " << x[1] << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::tensor<mito::DIM3>& x)
{
    os << "(" 
        << x[0] << ", " << x[1] << ", " << x[2] << "; "
        << x[3] << ", " << x[4] << ", " << x[5] << "; "
        << x[6] << ", " << x[7] << ", " << x[8] 
        << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::tensor<mito::DIM2>& x)
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

    for (auto i = 1; i < x.size(); ++i)
    {
        os << ", " << x[i];
    }

    os << "]";

    return os;
}

template<typename X, long unsigned int N>
std::ostream& operator<<(std::ostream& os, const std::array<X, N>& x){
    
    if (N == 0) {
        os << "[]";
        return os;
    }

    os << "[" << x[0];

    for (auto i = 1; i < N; ++i)
    {
        os << ", " << x[i];
    }

    os << "]";

    return os;
}

namespace mito {

    template <DIM D>
    int Factorial()
    {
        return D * Factorial<DIM(D - 1)>();
    }
    template <> 
    int Factorial<mito::DIM1>() {return 1;}

    real ComputeDeterminant(const tensor<mito::DIM4>& A)
    {
        return A[1] * A[11] * A[14] * A[4] 
                - A[1] * A[10] * A[15] * A[4] 
                - A[11] * A[13] * A[2] * A[4] 
                + A[10] * A[13] * A[3] * A[4] 
                - A[0] * A[11] * A[14] * A[5] 
                + A[0] * A[10] * A[15] * A[5] 
                + A[11] * A[12] * A[2] * A[5] 
                - A[10] * A[12] * A[3] * A[5] 
                - A[1] * A[11] * A[12] * A[6] 
                + A[0] * A[11] * A[13] * A[6] 
                + A[1] * A[10] * A[12] * A[7] 
                - A[0] * A[10] * A[13] * A[7] 
                - A[15] * A[2] * A[5] * A[8] 
                + A[14] * A[3] * A[5] * A[8] 
                + A[1] * A[15] * A[6] * A[8] 
                - A[13] * A[3] * A[6] * A[8] 
                - A[1] * A[14] * A[7] * A[8] 
                + A[13] * A[2] * A[7] * A[8] 
                + A[15] * A[2] * A[4] * A[9] 
                - A[14] * A[3] * A[4] * A[9] 
                - A[0] * A[15] * A[6] * A[9] 
                + A[12] * A[3] * A[6] * A[9] 
                + A[0] * A[14] * A[7] * A[9] 
                - A[12] * A[2] * A[7] * A[9];
    }

    real ComputeDeterminant(const tensor<mito::DIM3>& A)
    {
        return A[0] * (A[4] * A[8] - A[5] * A[7]) - A[1] * (A[3] * A[8] - A[5] * A[6]) +
               A[2] * (A[3] * A[7] - A[4] * A[6]);
    }

    real ComputeDeterminant(const tensor<mito::DIM2>& A)
    {
        return A[0] * A[3] - A[1] * A[2];
    }

    real ComputeInverse(const tensor<mito::DIM3>& A, tensor<mito::DIM3>& invA)
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

    real ComputeInverse(const tensor<mito::DIM2>& A, tensor<mito::DIM2>& invA)
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
