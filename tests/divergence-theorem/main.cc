#include <mito/base.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::math::function_t;
using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::geometry::point_t;
using mito::topology::vertex_t;
using mito::topology::segment_t;
using mito::topology::triangle_t;

int
main()
{
    // a scalar function
    auto f = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[0] * x[1], x[0] * x[0] };
    });

    // df/dx[0]
    auto Dx = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[1], 2.0 * x[0] };
    });

    // df/dx[1]
    auto Dy = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[0], 0.0 };
    });

    // instantiate a vector field
    auto field = mito::math::field(f, { Dx, Dy });

    // build a scalar field with divergence of field
    auto divergence = mito::math::div(field);

    // analytic solution
    // int_{int} (div f) = int_0^1 int_0^1 y dx dy = 0.5
    // int_{boundary} (f.n) = int_{right} (f.n) + int_{top} + int_{left} + int_{bot}
    // int_{right} (f.n) = int_{right} (f0) = int_0^1 xy|(x=1) dy = int_0^1 y dy = 0.5
    // int_{top} (f.n) = int_{top} (f1) = int_1^0 x^2 dx = int_0^1 x^2 dx = 1/3
    // int_{left} (f.n) = - int_{left} (f0) = int_0^1 xy|(x=0) dy = 0
    // int_{bot} (f.n) = - int_{bot} (f1) = - int_0^1 x^2 dx = - 1/3

    /**
     * Mesh with four elements:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    auto vertex0 = mito::geometry::vertex(point_t<2> { 0.0, 0.0 });
    auto vertex1 = mito::geometry::vertex(point_t<2> { 1.0, 0.0 });
    auto vertex2 = mito::geometry::vertex(point_t<2> { 1.0, 1.0 });
    auto vertex3 = mito::geometry::vertex(point_t<2> { 0.5, 0.5 });
    auto vertex4 = mito::geometry::vertex(point_t<2> { 0.0, 1.0 });

    auto segment0 = mito::topology::segment({ vertex0, vertex1 });
    auto segment1 = mito::topology::segment({ vertex1, vertex3 });
    auto segment2 = mito::topology::segment({ vertex3, vertex0 });
    auto segment3 = mito::topology::segment({ vertex1, vertex2 });
    auto segment4 = mito::topology::segment({ vertex2, vertex3 });
    auto segment5 = mito::topology::segment({ vertex4, vertex3 });
    auto segment6 = mito::topology::segment({ vertex2, vertex4 });
    auto segment7 = mito::topology::segment({ vertex4, vertex0 });

    auto element0 = mito::topology::triangle({ segment0, segment1, segment2 });
    auto element1 = mito::topology::triangle({ segment3, segment4, segment1 });
    auto element2 = mito::topology::triangle({ segment6, segment5, segment4 });
    auto element3 = mito::topology::triangle({ segment7, segment2, segment5 });

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::manifolds::ManifoldTri elementSet;
    auto bodyManifold = mito::manifolds::manifold<2>(
        mito::topology::element_vector_t<triangle_t> { element0, element1, element2, element3 });
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    real resultBody = bodyIntegrator.integrate(divergence);
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations (we might need std::inner_product to do the inner product)
    /*
    mito::manifolds::Manifold boundaryManifold(
        mito::topology::element_vector_t<segment_t> { &segment0, &segment3, &segment6, &segment7 });
    */
    // integrator on the bottom boundary
    auto boundaryBot =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment0 });
    auto boundaryBotIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryBot);

    // integrator on the right boundary
    auto boundaryRight =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment3 });
    auto boundaryRightIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryRight);

    // integrator on the top boundary
    auto boundaryTop =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment6 });
    auto boundaryTopIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryTop);

    // integrator on the left boundary
    auto boundaryLeft =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment7 });
    auto boundaryLeftIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryLeft);

    auto f0 = mito::math::field(f[0]);
    auto f1 = mito::math::field(f[1]);

    real resultBoundary =
        -boundaryBotIntegrator.integrate(f1) + boundaryRightIntegrator.integrate(f0)
        + boundaryTopIntegrator.integrate(f1) - boundaryLeftIntegrator.integrate(f0);

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    assert(std::fabs(resultBody - resultBoundary) < 1.e-15);

    // all done
    return 0;
}

// end of file
