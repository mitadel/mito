#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;


TEST(Quadrature, MeshSegment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<1>();

    // a 1D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);

    // a mesh of the segment (0, 1)
    auto vertex0 = geometry.node({ 0.0 });
    auto vertex1 = geometry.node({ 1.0 });
    auto segment0 = topology.segment({ vertex0, vertex1 });
    mesh.insert(segment0);
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, 12);

    // an integrator with degree of exactness 2 on segment (0, 1)
    auto manifold = mito::manifolds::manifold(tetra_mesh);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    // a scalar function
    auto f_exp = mito::math::field(mito::math::function(
        [](const mito::vector_t<1> & x) -> mito::scalar_t { return exp(-x[0]); }));

    // integrate exp(-x) on (0, 1)
    auto integral = integrator.integrate(f_exp);
    EXPECT_NEAR(integral, (exp(1) - 1) / exp(1), 1.e-13);
}


TEST(Quadrature, MeshTriangles)
{
    /**
     * Mesh with four cells:
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
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud_2D = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud_2D);

    // an empty mesh of simplicial topology in 2D
    auto mesh_2D = mito::mesh::mesh<mito::topology::triangle_t, 2>(geometry);

    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 1.0, 1.0 });
    auto vertex3 = geometry.node({ 0.5, 0.5 });
    auto vertex4 = geometry.node({ 0.0, 1.0 });

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex3 });
    auto segment8 = topology.segment({ vertex3, vertex2 });
    auto cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto segment9 = topology.segment({ vertex4, vertex0 });
    auto segment10 = topology.segment({ vertex0, vertex3 });
    auto segment11 = topology.segment({ vertex3, vertex4 });
    auto cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh_2D.insert(cell0);
    mesh_2D.insert(cell1);
    mesh_2D.insert(cell2);
    mesh_2D.insert(cell3);

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto bodyManifold = mito::manifolds::manifold(mesh_2D);
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
    EXPECT_DOUBLE_EQ(result, 1.0);

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
    EXPECT_DOUBLE_EQ(result, 0.5);

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
    EXPECT_DOUBLE_EQ(result, 0.25);

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
    EXPECT_DOUBLE_EQ(result, 1.0 / 3.0);

    // attach different coordinates (3D coordinates to the same points as above)
    // an empty cloud of points in 3D
    auto & point_cloud_3D = mito::geometry::point_cloud<3>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry_3D = mito::geometry::geometry(topology, point_cloud_3D);

    geometry_3D.node(vertex0, point_cloud_3D.point({ 0.0, 0.0, 0.0 }));
    geometry_3D.node(vertex1, point_cloud_3D.point({ 1.0, 0.0, 1.0 }));
    geometry_3D.node(vertex2, point_cloud_3D.point({ 1.0, 1.0, 1.0 }));
    geometry_3D.node(vertex3, point_cloud_3D.point({ 0.5, 0.5, 0.5 }));
    geometry_3D.node(vertex4, point_cloud_3D.point({ 0.0, 1.0, 0.0 }));

    // an empty mesh of simplicial topology in 3D
    auto mesh_3D = mito::mesh::mesh<mito::topology::triangle_t, 3>(geometry_3D);
    mesh_3D.insert(cell0);
    mesh_3D.insert(cell1);
    mesh_3D.insert(cell2);
    mesh_3D.insert(cell3);

    // instantiate an cell set with the same cells as above but the new coordinates map
    auto bodyManifold3D = mito::manifolds::manifold(mesh_3D);

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
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
    EXPECT_DOUBLE_EQ(result, 0.35355339059327384);
}

TEST(Quadrature, LoadMeshTriangles)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t, 2>(fileStream, geometry);
    // TOFIX: is it better that the cells and vertices are first fetched from mesh and then
    //  used to build the manifold?
    // instantiate a cell set as a collection of simplices and vertices.
    // const auto & cells = mesh.cells<2>();    // TODO: region label to fetch cells
    // auto manifold = mito::manifolds::manifold<2>(cells);
    auto manifold = mito::manifolds::manifold(mesh);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    EXPECT_NEAR(result, 0.946083, 1.e-7);
}

TEST(Quadrature, FlipSegment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a segment
    auto vertex0 = geometry.node({ 0.0, 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 1.0, 1.0 });
    auto segment0 = topology.segment({ vertex0, vertex1 });

    // the integrand
    auto f = mito::math::function([](const mito::vector_t<3> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // integrate the integrand on {segment0}
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    mesh.insert(segment0);
    auto manifold = mito::manifolds::manifold(mesh);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);
    auto result = integrator.integrate(f_cosine);

    // integrate the integrand on the opposite of {segment0}
    auto mesh_flip = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    mesh_flip.insert(topology.flip(segment0));
    auto manifold_flip = mito::manifolds::manifold(mesh_flip);
    auto integrator_flip = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold_flip);
    auto result_flip = integrator_flip.integrate(f_cosine);

    std::cout << "Integration of cos(x*y): Result = " << result << std::endl;
    std::cout << "Integration of cos(x*y): Result flipped = " << result_flip << std::endl;

    // expect to obtain a minus sign
    EXPECT_DOUBLE_EQ(result, -result_flip);
}

// end of file
