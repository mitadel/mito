#include "../../mito.h"
#include "../../mesh/Simplex.h"
#include "../../mesh/Mesh.h"
#include "../../mesh/ElementSet.h"
#include "../../math/Function.h"
#include "../../math/Field.h"
#include "../../quadrature/Integrator.h"

int
main()
{
    // load mesh
    mito::Mesh<2> mesh("square.summit");
    // instantiate an ElementSet as a collection of simplices and a vertex-coordinates mapping.
    const auto & elements = mesh.getEntities<2>(); //TODO: region label to fetch elements
    const auto & vertexPointMap = mesh.getVertexPointMap();
    mito::ElementSet elementSet(mesh.getEntities<2>(), mesh.getVertexPointMap());

    // a scalar function
    mito::Function<mito::vector_t<2>> f(
        [](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    // instantiate a scalar field
    mito::ScalarField f_cosine(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    mito::Integrator<
        mito::GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::triangle_t, 2>>
        integrator(elementSet);

    mito::real result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-7);

    return 0;
}
