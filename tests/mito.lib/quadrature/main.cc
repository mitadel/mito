#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;

TEST(Quadrature, TestQuadrature)
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

    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto point_cloud_2D = mito::geometry::point_cloud<2>();

    // an empty mesh of simplicial topology in 2D
    auto mesh_2D = mito::mesh::mesh<2, mito::topology::simplex_t>();

    auto vertex0 = topology.vertex();
    auto point0 = point_cloud_2D.point({ 0.0, 0.0 });
    mesh_2D.addVertex(vertex0, point0);
    auto vertex1 = topology.vertex();
    auto point1 = point_cloud_2D.point({ 1.0, 0.0 });
    mesh_2D.addVertex(vertex1, point1);
    auto vertex2 = topology.vertex();
    auto point2 = point_cloud_2D.point({ 1.0, 1.0 });
    mesh_2D.addVertex(vertex2, point2);
    auto vertex3 = topology.vertex();
    auto point3 = point_cloud_2D.point({ 0.5, 0.5 });
    mesh_2D.addVertex(vertex3, point3);
    auto vertex4 = topology.vertex();
    auto point4 = point_cloud_2D.point({ 0.0, 1.0 });
    mesh_2D.addVertex(vertex4, point4);

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex4, vertex3 });
    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex0 });

    auto element0 = topology.triangle({ segment0, segment1, segment2 });
    auto element1 = topology.triangle({ segment3, segment4, segment1 });
    auto element2 = topology.triangle({ segment6, segment5, segment4 });
    auto element3 = topology.triangle({ segment7, segment2, segment5 });

    mesh_2D.insert(element0);
    mesh_2D.insert(element1);
    mesh_2D.insert(element2);
    mesh_2D.insert(element3);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyManifold = mito::manifolds::manifold<2>(mesh_2D);
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
    EXPECT_NEAR(result, 0.946083, 1.e-3);

    // a vector function
    auto g = mito::math::function([](const mito::vector_t<2> & x) -> vector_t<2> {
        return { cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });
    // a vector field
    auto g_field = mito::math::field(g);
    // integrate the field
    auto resultVector = bodyIntegrator.integrate(g_field);
    // check the result
    EXPECT_TRUE((resultVector == mito::vector_t<2> { result, result }));

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
    EXPECT_NEAR(result, 1.0, 1.e-16);

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
    EXPECT_NEAR(result, 0.5, 1.e-16);

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
    EXPECT_NEAR(result, 0.25, 1.e-16);

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
    EXPECT_NEAR(result, 1.0 / 3.0, 1.e-16);

    // attach different coordinates (3D coordinates to the same points as above)
    // an empty cloud of points in 3D
    auto point_cloud_3D = mito::geometry::point_cloud<3>();

    // an empty mesh of simplicial topology in 3D
    auto mesh_3D = mito::mesh::mesh<3, mito::topology::simplex_t>();
    mesh_3D.addVertex(vertex0, point_cloud_3D.point({ 0.0, 0.0, 0.0 }));
    mesh_3D.addVertex(vertex1, point_cloud_3D.point({ 1.0, 0.0, 1.0 }));
    mesh_3D.addVertex(vertex2, point_cloud_3D.point({ 1.0, 1.0, 1.0 }));
    mesh_3D.addVertex(vertex3, point_cloud_3D.point({ 0.5, 0.5, 0.5 }));
    mesh_3D.addVertex(vertex4, point_cloud_3D.point({ 0.0, 1.0, 0.0 }));
    mesh_3D.insert(element0);
    mesh_3D.insert(element1);
    mesh_3D.insert(element2);
    mesh_3D.insert(element3);

    // instantiate an element set with the same elements as above but the new coordinates map
    auto bodyManifold3D = mito::manifolds::manifold<2>(mesh_3D);

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
    EXPECT_NEAR(result, 0.35355339059327384, 1.e-15);
}

// end of file
