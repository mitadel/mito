#include "../../mito.h"

int
main()
{
    mito::vector_t<2> vector1;
    assert(vector1 == vector1);

    mito::vector_t<2> vector2 = { 1.0, 2.0 };
    vector1 += vector2;
    assert(vector1 + vector2 == 2.0 * vector2);
    assert(vector2 - vector1 == 2.0 * (vector2 - vector1));
    assert((vector2 - vector2 == mito::vector_t<2> { 0, 0 }));

    mito::scalar_t<> a = 1.0;
    assert(vector2 * a == vector2);
    assert(vector2 * (-a) == -vector2);
    assert(vector2 / a == vector2);
    assert(vector2 / (-a) == -vector2);

    mito::vector_t<3> vector3 = { 1, 0, 0 };
    mito::vector_t<3> vector4 = { 0, 1, 0 };
    assert(vector3 * vector4 == 0.0);

    mito::scalar_t<> b(1.0);
    assert(2 * b == b + 1);
    assert(2 * b - true == b);

    // TODO: Add tests for all algebraic operators

    // all done
    return 0;
}