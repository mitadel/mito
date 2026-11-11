// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
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

// the material type
using material_t = mito::materials::thermal::ConstantIsotropic<2>;
// the model type
using model_t = mito::models::HeatDiffusion<material_t>;
// the euclidean metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the triangle type
using triangle_t = mito::geometry::triangle_t<2>;
// the segment type
using segment_t = mito::geometry::segment_t<2>;
// the node type
using node_t = mito::geometry::node_t<2>;
// the linear algebra backend type
using linalg_backend_t = mito::linalg::petsc::backend_t;
// the scalar type
using scalar_t = typename linalg_backend_t::scalar_type;

// the model parameters
struct Parameters {
    std::size_t n_steps = 80;
    int x_segments = 128;
    int y_segments = 128;

    scalar_t width = 1.;
    scalar_t conductivity = 0.5;
    scalar_t density = 1.25;
    scalar_t specific_heat = 2.0;
    scalar_t source_term = 0.0;
    scalar_t t_final = 1.0;
};

TEST(Timesteppers, CRANK_NICHOLSON_LOOP)
{
    // initialise PETSc environment
    auto petsc_env = mito::linalg::petsc::environment();

    // create the parameters
    const auto parameters = Parameters();

    // discretization parameters
    const auto dt = parameters.t_final / parameters.n_steps;
    const auto dx = parameters.width / parameters.x_segments;
    const auto dy = parameters.width / parameters.y_segments;

    // make a channel
    journal::info_t channel("tests.timesteppers.crank_nicholson_loop");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // define a mesh
    auto mesh = mito::mesh::mesh<triangle_t>();

    // create the nodes of the mesh
    auto node_grid = std::vector<std::vector<node_t>>{};

    // reserve space for the nodes
    node_grid.reserve(parameters.y_segments + 1);

    // loop over the y segments
    for (int j = 0; j <= parameters.y_segments; ++j) {
        // compute the y coordinate of the node
        const auto y = static_cast<scalar_t>(j) * dy;

        // create a row of nodes
        auto row = std::vector<node_t>{};

        // reserve space for the nodes in the row
        row.reserve(parameters.x_segments + 1);

        // loop over the x segments
        for (int i = 0; i <= parameters.x_segments; ++i) {
            // compute the x coordinate of the node
            const auto x = static_cast<scalar_t>(i) * dx;
            // create the node
            row.push_back(mito::geometry::node(coord_system, { x, y }));
        }

        // add the row of nodes to the node grid
        node_grid.push_back(std::move(row));
    }

    // loop over the segments and create the elements of the mesh
    for (int j = 0; j < parameters.y_segments; ++j) {
        // loop over the segments and create the elements of the mesh
        for (int i = 0; i < parameters.x_segments; ++i) {
            // get the nodes of the element
            const auto & node_00 = node_grid[j][i];
            const auto & node_10 = node_grid[j][i + 1];
            const auto & node_01 = node_grid[j + 1][i];
            const auto & node_11 = node_grid[j + 1][i + 1];

            // create the elements of the mesh; alternate the diagonal of the elements
            if ((i + j) % 2 == 0) {
                mesh.insert({ node_00, node_10, node_11 });
                mesh.insert({ node_00, node_11, node_01 });
            } else {
                mesh.insert({ node_00, node_10, node_01 });
                mesh.insert({ node_10, node_11, node_01 });
            }
        }
    }


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

    // the material type
    auto material = material_t(
        parameters.conductivity, parameters.density, parameters.specific_heat,
        parameters.source_term);

    // the model
    auto model = model_t(material);

    // the semi-discrete model
    auto semi_discrete_model = mito::fem::semi_discrete_model(function_space, model);

    // instantiate a PETSc Krylov solver for the time stepper
    auto solver = mito::linalg::petsc::ksp("time_stepper");

    // the initial condition field
    auto initial_condition_field =
        mito::functions::sin(std::numbers::pi * x) * mito::functions::sin(std::numbers::pi * y);

    // the time stepper
    auto time_stepper = mito::timesteppers::crank_nicholson<linalg_backend_t>(
        semi_discrete_model, std::move(solver), initial_condition_field, dt);

    // initialise the time stepper
    time_stepper.initialise();

    // loop over time
    for (std::size_t i = 0; i < parameters.n_steps; i++) {
        time_stepper.march();
    }

    // the solution field
    const auto & solution_field = time_stepper.solution_field();

    // the exact solution field
    auto u_ex = mito::functions::sin(std::numbers::pi * x)
              * mito::functions::sin(std::numbers::pi * y)
              * mito::functions::exp(
                    -2.0 * parameters.conductivity / (parameters.density * parameters.specific_heat)
                    * std::numbers::pi * std::numbers::pi * parameters.t_final);

    // compute the L2 error
    auto error_L2 = mito::fem::compute_l2_norm<2>(function_space, solution_field, u_ex);

    // report
    channel << "L2 error: " << error_L2 << journal::endl;

    // compute the H1 error
    auto error_H1 = mito::fem::compute_h1_norm<2, 2>(function_space, solution_field, u_ex);
    // report
    channel << "H1 error: " << error_H1 << journal::endl;

    // all done
    return;
}