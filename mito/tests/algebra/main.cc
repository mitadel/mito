#include "../../algebra/algebra.h"

int
main()
{
    mito::vector_t<2> vector1;
    std::cout << vector1 << std::endl;

    mito::vector_t<2> vector2 = { 1.0, 2.0 };
    std::cout << vector2 << std::endl;

    vector1 += vector2;
    vector1 += vector2;
    std::cout << vector1 << std::endl;

    mito::scalar_t a = 1.0;
    std::cout << a << std::endl;

    assert(vector1 == vector1);

    // all done
    return 0;
}