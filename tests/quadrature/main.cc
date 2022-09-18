#include <mito/base.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::geometry::point_t;
using mito::topology::triangle_t;

int
main()
{
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
    mito::topology::element_vector_t<triangle_t> elements = { element0, element1, element2,
                                                              element3 };

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyManifold = mito::manifolds::manifold<2>(elements);
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    // a scalar function
    auto f = mito::math::function([](const vector_t<2> & x) -> real { return cos(x[0] * x[1]); });
    // a scalar field
    auto f_field = mito::math::field(f);
    // integrate the field
    real result = bodyIntegrator.integrate(f_field);    // exact 0.946083...
    // report
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    // check the result
    assert(std::fabs(result - 0.946083) < 1.e-3);

    // a vector function
    auto g = mito::math::function([](const mito::vector_t<2> & x) -> vector_t<2> {
        return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });
    // a vector field
    auto g_field = mito::math::field(g);
    // integrate the field
    auto resultVector = bodyIntegrator.integrate(g_field);
    // check the result
    assert((resultVector == mito::vector_t<2> { result, result }));

    // a scalar function
    auto f_one =
        mito::math::function([]([[maybe_unused]] const vector_t<2> & x) -> real { return 1.0; });
    // a scalar field
    auto one = mito::math::field(f_one);
    // integrate the field
    result = bodyIntegrator.integrate(one);    // exact 1.0
    // report
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    // check the result
    assert(std::fabs(result - 1.0) < 1.e-16);

    // a scalar function
    auto f_linear = mito::math::function([](const vector_t<2> & x) -> real { return x[0]; });
    // a scalar field
    auto linear = mito::math::field(f_linear);
    // integrate the field
    result = bodyIntegrator.integrate(linear);    // exact 0.5
    // report
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    // check the result
    assert(std::fabs(result - 0.5) < 1.e-16);

    // a scalar function
    auto f_xy = mito::math::function([](const vector_t<2> & x) -> real { return x[0] * x[1]; });
    // a scalar field
    auto xy = mito::math::field(f_xy);
    // integrate the field
    result = bodyIntegrator.integrate(xy);    // exact 0.25
    // report
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    // check the result
    assert(std::fabs(result - 0.25) < 1.e-16);

    // a scalar function
    auto f_xx = mito::math::function([](const vector_t<2> & x) -> real { return x[0] * x[0]; });
    // a scalar field
    auto xx = mito::math::field(f_xx);
    // integrate the field
    result = bodyIntegrator.integrate(xx);    // exact 1.0/3.0
    // report
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    // check the result
    assert(std::fabs(result - 1.0 / 3.0) < 1.e-16);

    // attach different coordinates (3D coordinates to the same points as above)
    mito::geometry::point_cloud<3>::insert(vertex0, point_t<3> { 0.0, 0.0, 0.0 });
    mito::geometry::point_cloud<3>::insert(vertex1, point_t<3> { 1.0, 0.0, 1.0 });
    mito::geometry::point_cloud<3>::insert(vertex2, point_t<3> { 1.0, 1.0, 1.0 });
    mito::geometry::point_cloud<3>::insert(vertex3, point_t<3> { 0.5, 0.5, 0.5 });
    mito::geometry::point_cloud<3>::insert(vertex4, point_t<3> { 0.0, 1.0, 0.0 });

    // instantiate an element set with the same elements as above but the new coordinates map
    auto bodyManifold3D = mito::manifolds::manifold<3>(elements);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyIntegrator3D =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold3D);

    // a scalar function
    auto f_xy3D = mito::math::function([](const vector_t<3> & x) -> real { return x[0] * x[1]; });
    // a scalar field
    auto xy3D = mito::math::field(f_xy3D);
    // integrate the field
    result = bodyIntegrator3D.integrate(xy3D);    // exact 0.35355339059327384
    // report
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - 0.35355339059327384) << std::endl;
    // check the result
    assert(std::fabs(result - 0.35355339059327384) < 1.e-15);

    // all done
    return 0;
}

// end of file
