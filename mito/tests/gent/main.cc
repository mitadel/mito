#include "../../mito.h"
#include "../../materials/gent.h"

int
main(int argc, char ** argv)
{
    mito::Gent material(1.0 /*rho*/, 1.0 /*kappa*/, 1.0 /*mu*/, 1.0 /*Jm*/);

    mito::vector_t<3> u = { 0.0, 0.0, 0.0 };
    mito::matrix_t<3> Du = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
    mito::matrix_t<3> P = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    material.Constitutive<3>(u, Du, P);

    mito::vector_t<2> u2 = { 0.0, 0.0 };
    mito::matrix_t<2> Du2 = { 1.0, 0.0, 0.0, 1.0 };
    mito::matrix_t<2> P2 = { 0.0, 0.0, 0.0, 0.0 };
    material.Constitutive<2>(u2, Du2, P2);

    std::cout << "P = " << P << std::endl;
    std::cout << "P2 = " << P2 << std::endl;

    return 0;
}