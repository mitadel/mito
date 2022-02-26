#include "../../base.h"
#include "../../math.h"
#include "../../mesh.h"
#include "../../quadrature.h"

using mito::math::function_t;
using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::mesh::point_t;
using mito::mesh::vertex_t;
using mito::mesh::segment_t;
using mito::mesh::triangle_t;

int
main()
{
    // a scalar function
    auto f = mito::math::function([](const vector_t<2> & x) -> real { return cos(x[0] * x[1]); });

    // df/dx[0]
    auto Dfx = mito::math::function(
        [](const vector_t<2> & x) -> real { return -sin(x[0] * x[1]) * x[1]; });

    // df/dx[1]
    auto Dfy = mito::math::function(
        [](const vector_t<2> & x) -> real { return -sin(x[0] * x[1]) * x[0]; });

    // instantiate a scalar field
    // TOFIX: make this compile
    // auto f_cosine = mito::math::scalar_field(f, { Dfx, Dfy });
    auto f_cosine = mito::math::scalar_field_t<2>(f, { Dfx, Dfy });

    auto cosine_sum1 = mito::math::scalar_field(f + f);
    auto cosine_sum2 = mito::math::scalar_field(f_cosine + f_cosine);

    // a vector function
    auto g = mito::math::function([](const mito::vector_t<2> & x) -> vector_t<2> {
        return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });

    // dg/dx[0]
    auto Dgx = mito::math::function([](const mito::vector_t<2> & x) -> vector_t<2> {
        return { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[1] };
    });

    // dg/dx[1]
    auto Dgy = mito::math::function([](const mito::vector_t<2> & x) -> vector_t<2> {
        return { -sin(x[0] * x[1]) * x[0], -sin(x[0] * x[1]) * x[0] };
    });

    // its partial derivatives
    // std::array<function_t<vector_t<2>, vector_t<2>>, 2> Dg = { Dgx, Dgy };
    // instantiate a vector field
    // mito::math::vector_field_t<2 /* D */, 2 /* N */> cosineVector(g, Dg);
    // TOFIX: make this compile
    // auto cosineVector = mito::math::vector_field(f, { Dfx, Dfy });
    auto cosineVector = mito::math::vector_field_t<2 /* D */, 2 /* N */>(g, { Dgx, Dgy });

    // a point in the reference configuration
    vector_t<2> x = { 0.0, 0.0 };

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << x << " : " << f_cosine(x)
              << std::endl;
    std::cout << "Evaluating gradient of cosine function at X = " << x << " : "
              << mito::math::grad(f_cosine, x) << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at x = " << x << " : "
              << mito::math::div(cosineVector, x) << std::endl;

    auto gradient = mito::math::grad(f_cosine);
    // evaluate divergence
    auto divergence = mito::math::div(cosineVector);

    std::cout << "Evaluating gradient of cosine function at x = " << x << " : " << gradient(x)
              << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at X = " << x << " : "
              << divergence(x) << std::endl;

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

    auto vertices = mito::mesh::vertex_set<2>();

    vertex_t vertex0;
    vertices.insert(vertex0, point_t<2> { 0.0, 0.0 });
    vertex_t vertex1;
    vertices.insert(vertex1, point_t<2> { 1.0, 0.0 });
    vertex_t vertex2;
    vertices.insert(vertex2, point_t<2> { 1.0, 1.0 });
    vertex_t vertex3;
    vertices.insert(vertex3, point_t<2> { 0.5, 0.5 });
    vertex_t vertex4;
    vertices.insert(vertex4, point_t<2> { 0.0, 1.0 });

    auto segment0 = mito::mesh::segment({ &vertex0, &vertex1 });
    auto segment1 = mito::mesh::segment({ &vertex1, &vertex3 });
    auto segment2 = mito::mesh::segment({ &vertex3, &vertex0 });
    auto segment3 = mito::mesh::segment({ &vertex1, &vertex2 });
    auto segment4 = mito::mesh::segment({ &vertex2, &vertex3 });
    auto segment5 = mito::mesh::segment({ &vertex4, &vertex3 });
    auto segment6 = mito::mesh::segment({ &vertex2, &vertex4 });
    auto segment7 = mito::mesh::segment({ &vertex4, &vertex0 });

    auto element0 = mito::mesh::triangle({ &segment0, &segment1, &segment2 });
    auto element1 = mito::mesh::triangle({ &segment3, &segment4, &segment1 });
    auto element2 = mito::mesh::triangle({ &segment6, &segment5, &segment4 });
    auto element3 = mito::mesh::triangle({ &segment7, &segment2, &segment5 });
    std::vector<triangle_t *> elements = { &element0, &element1, &element2, &element3 };

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyElementSet = mito::mesh::element_set(elements, vertices);
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyElementSet);

    real result = bodyIntegrator.integrate(f_cosine);    // exact 0.946083...
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-3);

    auto resultVector = bodyIntegrator.integrate(cosineVector);
    assert((resultVector == mito::vector_t<2> { result, result }));

    // instantiate a scalar function object
    auto one = mito::math::scalar_field([]([[maybe_unused]] const vector_t<2> & x) { return 1.0; });
    result = bodyIntegrator.integrate(one);    // exact 1.0
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    assert(std::fabs(result - 1.0) < 1.e-16);

    // instantiate a scalar function object
    auto linear = mito::math::scalar_field([](const vector_t<2> & x) { return x[0]; });
    result = bodyIntegrator.integrate(linear);    // exact 0.5
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    assert(std::fabs(result - 0.5) < 1.e-16);

    // instantiate a scalar function object
    auto xy = mito::math::scalar_field([](const vector_t<2> & x) { return x[0] * x[1]; });
    result = bodyIntegrator.integrate(xy);    // exact 0.25
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    assert(std::fabs(result - 0.25) < 1.e-16);

    // instantiate a scalar function object
    auto xx = mito::math::scalar_field([](const vector_t<2> & x) { return x[0] * x[0]; });
    result = bodyIntegrator.integrate(xx);    // exact 1.0/3.0
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    assert(std::fabs(result - 1.0 / 3.0) < 1.e-16);

    // attach different coordinates (3D coordinates to the same vertices as above)
    auto vertices3D = mito::mesh::vertex_set<3>();
    vertices3D.insert(vertex0, point_t<3> { 0.0, 0.0, 0.0 });
    vertices3D.insert(vertex1, point_t<3> { 1.0, 0.0, 1.0 });
    vertices3D.insert(vertex2, point_t<3> { 1.0, 1.0, 1.0 });
    vertices3D.insert(vertex3, point_t<3> { 0.5, 0.5, 0.5 });
    vertices3D.insert(vertex4, point_t<3> { 0.0, 1.0, 0.0 });

    // instantiate an element set with the same elements as above but the new coordinates map
    auto bodyElementSet3D = mito::mesh::element_set(elements, vertices3D);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyIntegrator3D =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyElementSet3D);

    // instantiate a scalar function object
    auto xy3D = mito::math::scalar_field([](const vector_t<3> & x) -> real { return x[0] * x[1]; });
    result = bodyIntegrator3D.integrate(xy3D);    // exact 0.35355339059327384
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - 0.35355339059327384) << std::endl;
    assert(std::fabs(result - 0.35355339059327384) < 1.e-15);

    return 0;
}

// end of file
