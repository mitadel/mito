#include "../../mito.h"

int
main()
{
    mito::vector_t<2> vector1;
    std::cout << vector1 << std::endl;
    assert(vector1 == vector1);

    mito::vector_t<2> vector2 = { 1.0, 2.0 };
    std::cout << vector2 << std::endl;

    vector1 += vector2;
    vector1 += vector2;
    std::cout << vector1 << std::endl;

    mito::scalar_t a = 1.0;
    std::cout << a << std::endl;

    mito::vector_t<3> vector3 = { 1, 0, 0 };
    mito::vector_t<3> vector4 = { 0, 1, 0 };
    assert(vector3 * vector4 == 0.0);

    // TODO: Add all test for algebraic operators

    // all done
    return 0;
}