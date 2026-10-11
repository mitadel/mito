// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the material type
using material_t = mito::materials::thermal::ConstantIsotropic<2>;
// the model type
using model_t = mito::models::HeatDiffusion<material_t>;


TEST(Fem, IsoparametricTriangle)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // an atlas under the coordinate system
    auto atlas = mito::manifolds::atlas<cell_t>(coord_system);

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // make a geometric simplex
    auto triangle = mito::geometry::triangle(node_0, node_1, node_2);

    // make a manifold element from the segment
    auto element = mito::manifolds::parametrized_element(
        triangle, atlas.parametrization(triangle), metric_space_t::w);

    // constant isotropic material with density = 1.25, conductivity = 0.5, specific heat = 2,
    constexpr auto conductivity = mito::real(0.5);
    constexpr auto density = mito::real(1.25);
    constexpr auto specific_heat = mito::real(2.0);
    constexpr auto source_term = mito::real(1.0);

    // the material
    auto material = material_t(conductivity, density, specific_heat, source_term);

    // the model
    auto model = model_t(material);

    // the model stiffness block
    auto model_stiffness_block = model.getBlock<mito::models::ParabolicPDE::Stiffness>();

    // the model capacitance block
    auto model_capacitance_block = model.getBlock<mito::models::ParabolicPDE::Capacitance>();

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();

        // the degree of the finite element
        constexpr int degree = 1;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // a finite element
        auto element_p1 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2 });

        // the analytical elementary stiffness matrix
        auto analytical_stiffness_matrix =
            conductivity / 2.0
            * mito::tensor::matrix_t<3>{ 2.0, -1.0, -1.0, -1.0, 1.0, 0.0, -1.0, 0.0, 1.0 };

        // the analytical elementary capacitance matrix
        auto analytical_capacitance_matrix =
            density * specific_heat / 24.0
            * mito::tensor::matrix_t<3>{ 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0 };

        // the model elementary stiffness matrix
        auto model_stiffness_matrix = model_stiffness_block.compute(element_p1);

        // the model elementary capacitance matrix
        auto model_capacitance_matrix = model_capacitance_block.compute(element_p1);

        // compute the stiffness error
        auto error_stiffness =
            mito::tensor::norm(model_stiffness_matrix - analytical_stiffness_matrix);

        // compute the capacitance error
        auto error_capacitance =
            mito::tensor::norm(model_capacitance_matrix - analytical_capacitance_matrix);

        // check the error is zero to machine precision
        EXPECT_DOUBLE_EQ(0.0, error_stiffness);
        // check the error is reasonable
        EXPECT_NEAR(0.0, error_capacitance, 1.e-15);
    }

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        auto discretization_node_2 = discretization_node_t();
        auto discretization_node_3 = discretization_node_t();
        auto discretization_node_4 = discretization_node_t();
        auto discretization_node_5 = discretization_node_t();

        // the degree of the finite element
        constexpr int degree = 2;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // a finite element
        auto element_p2 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1, discretization_node_2,
                       discretization_node_3, discretization_node_4, discretization_node_5 });

        // the analytical elementary stiffness matrix
        auto analytical_stiffness_matrix =
            conductivity * mito::tensor::matrix_t<6>{
            1.0,        1.0 / 6.0,  1.0 / 6.0,  -2.0 / 3.0, 0.0,        -2.0 / 3.0,
            1.0 / 6.0,  1.0 / 2.0,  0.0,        -2.0 / 3.0, 0.0,        0.0,
            1.0 / 6.0,  0.0,        1.0 / 2.0,  0.0,        0.0,        -2.0 / 3.0,
            -2.0 / 3.0, -2.0 / 3.0, 0.0,        8.0 / 3.0,  -4.0 / 3.0, 0.0,
            0.0,        0.0,        0.0,        -4.0 / 3.0, 8.0 / 3.0,  -4.0 / 3.0,
            -2.0 / 3.0, 0.0,        -2.0 / 3.0, 0.0,        -4.0 / 3.0, 8.0 / 3.0
        };

        // the analytical elementary capacitance matrix
        auto analytical_capacitance_matrix =
            density * specific_heat
            * mito::tensor::matrix_t<6>{
                  1.0 / 60.0,   -1.0 / 360.0, -1.0 / 360.0, 0.0,         -1.0 / 90.0, 0.0,
                  -1.0 / 360.0, 1.0 / 60.0,   -1.0 / 360.0, 0.0,         0.0,         -1.0 / 90.0,
                  -1.0 / 360.0, -1.0 / 360.0, 1.0 / 60.0,   -1.0 / 90.0, 0.0,         0.0,
                  0.0,          0.0,          -1.0 / 90.0,  4.0 / 45.0,  2.0 / 45.0,  2.0 / 45.0,
                  -1.0 / 90.0,  0.0,          0.0,          2.0 / 45.0,  4.0 / 45.0,  2.0 / 45.0,
                  0.0,          -1.0 / 90.0,  0.0,          2.0 / 45.0,  2.0 / 45.0,  4.0 / 45.0
              };

        // the model elementary stiffness matrix
        auto model_stiffness_matrix = model_stiffness_block.compute(element_p2);

        // the model elementary capacitance matrix
        auto model_capacitance_matrix = model_capacitance_block.compute(element_p2);

        // compute the stiffness error
        auto error_stiffness =
            mito::tensor::norm(model_stiffness_matrix - analytical_stiffness_matrix);

        // compute the capacitance error
        auto error_capacitance =
            mito::tensor::norm(model_capacitance_matrix - analytical_capacitance_matrix);

        // check the error is reasonable
        EXPECT_NEAR(0.0, error_stiffness, 1.e-14);
        EXPECT_NEAR(0.0, error_capacitance, 1.e-14);
    }

    // all done
    return;
}