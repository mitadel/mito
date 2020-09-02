#include <functional>
#include <iostream>
#include <array>

#define TEMPLATE 1

namespace mito {
    typedef double real;
    typedef std::array<real, 3> vector3D;
    typedef std::array<real, 2> vector2D;
    // should we call it 'field' instead of 'function'? 
    #if TEMPLATE
    template <typename X, typename Y>
    using function = Y(*)(const X&, real);
    #else 
    // the following two lines are equivalent
    //typedef std::function<vector3D(const vector3D&, real)> function;
    using function = vector3D(*)(const vector3D&, real);
    #endif
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

mito::vector2D myFunctionC(const mito::vector3D& x, mito::real t)
{
    return {(x[0]-1)*(x[1]-1), 0.0}; 
}

mito::real myFunctionVector2DtoReal(const mito::vector2D& x, mito::real t)
{
    return (x[0]-1)*(x[1]-1); 
}

#if TEMPLATE
template <typename X, typename Y>
int AddDirichletBC(std::string boundaryLabel, const mito::function<X, Y> & myF) 
#else
int AddDirichletBC(std::string boundaryLabel, const mito::function & myF) 
#endif
{
    std::cout << "Setting Dirichlet BC on " << boundaryLabel << std::endl;

#if TEMPLATE
    X x;
    std::fill(x.begin(), x.end(), 0.0);
#else
    mito::vector3D x = {0.0, 0.0, 0.0};
#endif
    mito::real t = 0.0;

    std::cout << "Evaluating BC function at " << x << std::endl;
    Y y = myF(x, t);
    std::cout << "myF(x, t) = " << myF(x, t) << std::endl;

    return 0;
}

int main() {
    
    //https://stackoverflow.com/questions/4295432/typedef-function-pointer
    //https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

    AddDirichletBC("boundary subset A", &myFunctionA);

    #if TEMPLATE
    mito::function<mito::vector3D,mito::vector3D> myFunction{ 
    #else
    mito::function myFunction{ 
    #endif
        [](const mito::vector3D& x, mito::real t){ 
        return mito::vector3D{(x[0]-1)*(x[1]+1), 0.0, 0.0};
        }
    };
    AddDirichletBC("boundary subset B", myFunction);

    #if TEMPLATE
    mito::function<mito::vector3D,mito::vector2D> myFunction3DtoReal{ 
        [](const mito::vector3D& x, mito::real t){ 
        return mito::vector2D{(x[0]-1)*(x[1]+1), 0.0};
        }
    };
    AddDirichletBC("boundary subset: function vector3D to real", myFunction);

    AddDirichletBC("boundary subset: function vector3D to vector2D", &myFunctionC);

    AddDirichletBC("boundary subset: function vector2D to real", &myFunctionVector2DtoReal);

    mito::function<mito::vector2D,mito::real> myOtherFunctionVector2DtoReal{ 
        [](const mito::vector2D& x, mito::real t){ 
        return (x[0]-1)*(x[1]+1);
        }
    };
    AddDirichletBC("boundary subset: other function vector2D to real", myOtherFunctionVector2DtoReal);

    mito::function<mito::real,mito::real> myFunctionRealtoReal( 
        [](const mito::real& x, mito::real t){ 
        return (x-1);
        }
    );
    AddDirichletBC("boundary subset: other function vector2D to real", myOtherFunctionVector2DtoReal);

    #else
    // for some reason this does not work with template implementation
    // lambda function 
    AddDirichletBC("boundary subset C", {
        [](const mito::vector3D& x, mito::real t)
        {return mito::vector3D{(x[0]-1)*(x[1]+1), 0.0, 0.0};}});
    #endif

    return 0;
}