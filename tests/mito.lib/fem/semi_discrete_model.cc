// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the material type
using material_t = mito::materials::thermal::ConstantIsotropic<2>;
// the model type
using model_t = mito::models::HeatDiffusion<material_t>;
// the euclidean metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;

// simplicial cells in 2D
using cell_t = mito::geometry::triangle_t<2>;
// first degree finite elements
constexpr int degree = 1;
// assemble the finite element type
using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;
// the x scalar field in 2D
constexpr auto x = mito::geometry::cartesian<2>::x;
// the y scalar field in 2D
constexpr auto y = mito::geometry::cartesian<2>::y;

TEST(Models, SEMI_DISCRETE_MODEL)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a square in 2D
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, coord_system);

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system, metric_space_t::w);

    // TOFIX: it should not be mandatory to set constraints to create a function space, let's remove
    // this bit once we implement constraints properly
    // get the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the zero field
    auto zero = mito::functions::zero<coordinates_t>;

    // set homogeneous Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // the function space (linear elements on the manifold)
    auto function_space = mito::fem::function_space<finite_element_t>(manifold, constraints);

    // constant isotropic material with density = 1.25, conductivity = 0.5, specific heat = 2,
    constexpr auto conductivity = mito::real(0.5);
    constexpr auto density = mito::real(1.25);
    constexpr auto specific_heat = mito::real(2.0);
    constexpr auto source_term = mito::real(1.0);

    // the material type
    auto material = material_t(conductivity, density, specific_heat, source_term);

    // the model
    auto model = model_t(material);

    // the semi-discrete weakform
    auto weakform = mito::fem::semi_discrete_weakform(model);

    // the semi-discrete model
    auto semi_discrete_model = mito::fem::semi_discrete_model(function_space, model);

    // all done
    return;
}