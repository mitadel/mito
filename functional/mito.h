#include <array>
#include <functional>
#include <iostream>

//https://stackoverflow.com/questions/4295432/typedef-function-pointer
//https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

namespace mito {
    // typedef for scalars
    typedef double real;
    // typedef for vectors
    template <size_t D>
    using vector = std::array<real, D>;
    // typedef for tensors
    template <size_t D>
    using tensor = std::array<real, D * D>;
    // templatized typedef for fields
    // should we call it 'field' instead of 'function'? 
    template <typename X, typename Y>
    using function = Y(*)(const X&, real);
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

namespace mito {
template <typename X, typename Y>
int AddDirichletBC(std::string boundaryLabel, const function<X, Y> & myF) 
{
    std::cout << "Setting Dirichlet BC on " << boundaryLabel << std::endl;

    X x;
    std::fill(x.begin(), x.end(), 0.0);

    real t = 0.0;

    std::cout << "\tEvaluating BC function at " << x << std::endl;

    auto y = myF(x, t);

    std::cout << "\tmyF(x, t) = " << myF(x, t) << std::endl;

    return 0;
}
}