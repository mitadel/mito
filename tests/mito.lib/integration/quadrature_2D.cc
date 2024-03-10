// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;

// alias for a set of cartesian coordinates in 2D
using coordinates_2D_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;
// alias for a set of cartesian coordinates in 3D
using coordinates_3D_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;


TEST(Quadrature, Square)
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

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud_2D);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 2D
    auto mesh_2D = mito::mesh::mesh<mito::topology::triangle_t, 2>(geometry);

    // create nodes
    auto vertex0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 1.0, 1.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.5, 0.5 });
    auto vertex4 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

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
    auto bodyManifold = mito::manifolds::manifold(mesh_2D, coord_system);
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    // a scalar field
    auto f = mito::manifolds::field(
        [](const coordinates_2D_t & x) -> real { return std::cos(x[0] * x[1]); });
    // integrate the field
    real result = bodyIntegrator.integrate(f);
    // the exact solution
    real exact = 0.9460830607878437;
    // report
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;
    // check the result
    EXPECT_NEAR(result, exact, 1.e-3);

    // a scalar field
    auto f_one = mito::manifolds::field([](const coordinates_2D_t &) -> real { return 1.0; });
    // integrate the field
    result = bodyIntegrator.integrate(f_one);    // exact 1.0
    // report
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 1.0);

    // a scalar field
    auto f_linear = mito::manifolds::field([](const coordinates_2D_t & x) -> real { return x[0]; });
    // integrate the field
    result = bodyIntegrator.integrate(f_linear);    // exact 0.5
    // report
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 0.5);

    // a scalar function
    auto f_xy =
        mito::manifolds::field([](const coordinates_2D_t & x) -> real { return x[0] * x[1]; });
    // integrate the field
    result = bodyIntegrator.integrate(f_xy);    // exact 0.25
    // report
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 0.25);

    // a scalar function
    auto f_xx =
        mito::manifolds::field([](const coordinates_2D_t & x) -> real { return x[0] * x[0]; });
    // integrate the field
    result = bodyIntegrator.integrate(f_xx);    // exact 1.0/3.0
    // report
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 1.0 / 3.0);

#if 0    // TOFIX
    // attach different coordinates (3D coordinates to the same points as above)
    // an empty cloud of points in 3D
    auto & point_cloud_3D = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry_3D = mito::geometry::geometry(topology, point_cloud_3D);

    // a Euclidean coordinate system in 3D
    auto coord_system_3D = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    auto point0_3D = point_cloud_3D.point();
    auto point1_3D = point_cloud_3D.point();
    auto point2_3D = point_cloud_3D.point();
    auto point3_3D = point_cloud_3D.point();
    auto point4_3D = point_cloud_3D.point();
    coord_system_3D.place(point0_3D, { 0.0, 0.0, 0.0 });
    coord_system_3D.place(point1_3D, { 1.0, 0.0, 1.0 });
    coord_system_3D.place(point2_3D, { 1.0, 1.0, 1.0 });
    coord_system_3D.place(point3_3D, { 0.5, 0.5, 0.5 });
    coord_system_3D.place(point4_3D, { 0.0, 1.0, 0.0 });

    geometry_3D.node(vertex0, point0_3D);
    geometry_3D.node(vertex1, point1_3D);
    geometry_3D.node(vertex2, point2_3D);
    geometry_3D.node(vertex3, point3_3D);
    geometry_3D.node(vertex4, point4_3D);

    // an empty mesh of simplicial topology in 3D
    auto mesh_3D = mito::mesh::mesh<mito::topology::triangle_t, 3>(geometry_3D);
    mesh_3D.insert(cell0);
    mesh_3D.insert(cell1);
    mesh_3D.insert(cell2);
    mesh_3D.insert(cell3);

    // instantiate an cell set with the same cells as above but the new coordinates map
    auto bodyManifold3D = mito::manifolds::manifold(mesh_3D, coord_system_3D);

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto bodyIntegrator3D =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold3D);

    // a scalar field
    auto f_xy3D = mito::manifolds::field([](const coordinates_3D_t & x) -> real { return x[0] * x[1]; });
    // integrate the field
    result = bodyIntegrator3D.integrate(f_xy3D);
    // the exact solution
    exact = 0.35355339059327384;
    // report
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, exact);
#endif
}

// end of file
