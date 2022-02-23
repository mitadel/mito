#include "../../base.h"
#include "../../mesh.h"
#include "../../math.h"
#include "../../quadrature.h"

int
main()
{
    // load mesh
    auto mesh = mito::mesh::mesh<2>("square.summit");
    // instantiate a element set as a collection of simplices and vertices.
    const auto & elements = mesh.elements<2>(); //TODO: region label to fetch elements
    const auto & vertices = mesh.vertices();
    auto elementSet = mito::mesh::element_set(elements, vertices);

    // instantiate a scalar field
    auto f_cosine = 
        mito::math::scalar_field([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(elementSet);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-7);

    return 0;
}
