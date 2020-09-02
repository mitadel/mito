#include "../mito.h"

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

int main() {
    
    mito::AddDirichletBC("boundary subset A", &myFunctionA);

    mito::field<mito::vector<3>, mito::vector<3> > myFunctionB{ 
        [](const mito::vector<3>& x, mito::real t){ 
        return mito::vector<3>{(x[0]-1)*(x[1]+1), 0.0, 0.0};
        }
    };
    mito::AddDirichletBC("boundary subset B", myFunctionB);

    mito::AddDirichletBC("boundary subset: field vector3D to vector2D", &myFunctionC);

    mito::field<mito::vector<3>,mito::vector<2> > myFunction3DtoReal{ 
        [](const mito::vector<3>& x, mito::real t){ 
        return mito::vector<2>{(x[0]-1)*(x[1]+1), 0.0};
        }
    };
    mito::AddDirichletBC("boundary subset: field vector2D to real", &myFunctionVector2DtoReal);

    mito::field<mito::vector<2>,mito::real> myOtherFunctionVector2DtoReal{ 
        [](const mito::vector<2>& x, mito::real t){ 
        return (x[0]-1)*(x[1]+1);
        }
    };
    mito::AddDirichletBC("boundary subset: other field vector2D to real", myOtherFunctionVector2DtoReal);

    mito::AddDirichletBC("boundary subset: tensor3D to real", &myFunctionTensor3DtoReal);

    return 0;
}