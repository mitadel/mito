#include "../../base.h"
#include "../../mesh.h"
#include "../../manifolds.h"
#include "../../math.h"
#include "../../quadrature.h"

int
main()
{
    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::mesh::summit<2>(fileStream);
    // instantiate a element set as a collection of simplices and vertices.
    const auto & elements = mesh.elements<2>();    // TODO: region label to fetch elements
    auto elementSet = mito::manifolds::manifold<2>(elements);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(elementSet);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-7);

    return 0;
}
