#include "../../base.h"
#include "../../mesh.h"
#include "../../math.h"
#include "../../quadrature.h"

int
main()
{
    // load mesh
    mito::mesh::Mesh<2> mesh("square.summit");
    // instantiate an ElementSet as a collection of simplices and a vertex-coordinates mapping.
    const auto & elements = mesh.elements<2>(); //TODO: region label to fetch elements
    const auto & vertices = mesh.vertices();
    mito::mesh::ElementSet elementSet(elements, vertices);

    // instantiate a scalar field
    mito::math::scalar_field_t<2> f_cosine(
        [](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    mito::quadrature::integrator_t<
        mito::quadrature::GAUSS, 2 /* degree of exactness */, mito::mesh::ElementSet<mito::mesh::triangle_t, 2>>
        integrator(elementSet);

    mito::real result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-7);

    return 0;
}
