#include <functional>
#include <iostream>
#include <array>

namespace mito {
    typedef double real;
    typedef std::array<real, 3> vector3D;
    typedef std::array<real, 2> vector2D;
    typedef std::function<vector3D(const vector3D&, real)> function;
    //template <typename T, typename S>
    //using function = S(*)(const T&, real);
}

std::ostream& operator<<(std::ostream& os, const mito::vector3D& x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mito::vector2D& x)
{
    os << "(" << x[0] << ", " << x[1] << ")";
    return os;
}

mito::vector3D myFunctionA(const mito::vector3D& x, mito::real t)
{
    return {(x[0]-1)*(x[1]-1), 0.0, 0.0}; 
}

int AddDirichletBC(std::string boundaryLabel, const mito::function & myF) 
{
    std::cout << "Setting Dirichlet BC on " << boundaryLabel << std::endl;

    mito::vector3D x = {0.0, 0.0, 0.0};
    mito::real t = 0.0;

    std::cout << "Evaluating BC function at " << x << std::endl;
    std::cout << "myF(x, t) = " << myF(x, t) << std::endl;

    return 0;
}

int main() {
    
    //https://stackoverflow.com/questions/4295432/typedef-function-pointer
    //https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

    AddDirichletBC("boundary subset A", myFunctionA);

    mito::function myFunction{ [](const mito::vector3D& x, mito::real t){ 
        return mito::vector3D{(x[0]-1)*(x[1]+1), 0.0, 0.0};
        }
    };
    AddDirichletBC("boundary subset B", myFunction);

    // lambda function
    AddDirichletBC("boundary subset C", {[](const mito::vector3D& x, mito::real t)
        {return mito::vector3D{(x[0]-1)*(x[1]+1), 0.0, 0.0};}});

    return 0;
}