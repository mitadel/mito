#include <functional>
#include <iostream>
#include <array>

namespace mito {
    typedef double real;

    template <size_t D>
    using vector = std::array<real, D>;
    template <size_t D>
    using tensor = std::array<real, D * D>;

    // should we call it 'field' instead of 'function'? 
    template <typename X, typename Y>
    using function = Y(*)(const X&, real);

}

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

mito::vector<3> myFunctionA(const mito::vector<3>& x, mito::real t)
{
    return {(x[0]-1)*(x[1]-1), 0.0, 0.0}; 
}

mito::vector<2> myFunctionC(const mito::vector<3>& x, mito::real t)
{
    return {(x[0]-1)*(x[1]-1), 0.0}; 
}

mito::real myFunctionVector2DtoReal(const mito::vector<2>& x, mito::real t)
{
    return (x[0]-1)*(x[1]-1); 
}

mito::real myFunctionTensor3DtoReal(const mito::tensor<3>& x, mito::real t)
{
    return x[0] + x[4] + x[8]; 
}

template <typename X, typename Y>
int AddDirichletBC(std::string boundaryLabel, const mito::function<X, Y> & myF) 
{
    std::cout << "Setting Dirichlet BC on " << boundaryLabel << std::endl;

    X x;
    std::fill(x.begin(), x.end(), 0.0);

    mito::real t = 0.0;

    std::cout << "Evaluating BC function at " << x << std::endl;

    auto y = myF(x, t);

    std::cout << "myF(x, t) = " << myF(x, t) << std::endl;

    return 0;
}

int main() {
    
    //https://stackoverflow.com/questions/4295432/typedef-function-pointer
    //https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

    AddDirichletBC("boundary subset A", &myFunctionA);

    mito::function<mito::vector<3>, mito::vector<3> > myFunction{ 
        [](const mito::vector<3>& x, mito::real t){ 
        return mito::vector<3>{(x[0]-1)*(x[1]+1), 0.0, 0.0};
        }
    };
    AddDirichletBC("boundary subset B", myFunction);

    mito::function<mito::vector<3>,mito::vector<2> > myFunction3DtoReal{ 
        [](const mito::vector<3>& x, mito::real t){ 
        return mito::vector<2>{(x[0]-1)*(x[1]+1), 0.0};
        }
    };
    AddDirichletBC("boundary subset: function vector3D to real", myFunction);

    AddDirichletBC("boundary subset: function vector3D to vector2D", &myFunctionC);

    AddDirichletBC("boundary subset: function vector2D to real", &myFunctionVector2DtoReal);

    mito::function<mito::vector<2>,mito::real> myOtherFunctionVector2DtoReal{ 
        [](const mito::vector<2>& x, mito::real t){ 
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

    AddDirichletBC("boundary subset: tensor3D to real", &myFunctionTensor3DtoReal);

    return 0;
}