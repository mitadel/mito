#include <functional>
#include <iostream>
#include <array>

namespace mito {
    typedef double real;
    typedef std::array<real, 3> vector;
    typedef std::function<vector(const mito::vector&, mito::real)> function;
    //using function = mito::vector(*)(const mito::vector&, mito::real);
}

int anwar(const mito::function & myF) 
{
    mito::vector x = {0.0, 0.0, 0.0};
    mito::real t = 0.0;

    std::cout << "myF(x, t) = " << myF(x, t)[0] 
              << "\t" << myF(x, t)[1] 
              << "\t" << myF(x, t)[2] 
              << std::endl;

    return 0;
}

int main() {
    
    //https://stackoverflow.com/questions/4295432/typedef-function-pointer
    //https://stackoverflow.com/questions/7787500/how-to-write-a-function-that-takes-a-functor-as-an-argument

    mito::function myFunction{ [](const mito::vector& x, mito::real t){ 
        return mito::vector{(x[0]-1)*(x[1]+1), 0.0, 0.0};
        }
    };

    anwar(myFunction);

    anwar({[](const mito::vector& x, mito::real t)
        {return mito::vector{(x[0]-1)*(x[1]+1), 0.0, 0.0};}});

    return 0;
}