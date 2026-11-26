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
// the backend type
using backend_t = mito::linalg::petsc::backend_t;


// check that {A_problem} and {A_model} act identically on {x}
template <class matrixT, class vectorT>
auto
expect_same_action(
    const matrixT & A_problem, const matrixT & A_model, const vectorT & x, int n_equations) -> void
{
    // y_problem = A_problem * x
    auto y_problem = mito::linalg::petsc::vector("y_problem", n_equations);
    A_problem.mult_vector(x, y_problem);

    // y_model = A_model * x
    auto y_model = mito::linalg::petsc::vector("y_model", n_equations);
    A_model.mult_vector(x, y_model);

    // the reference norm (guard against a trivially zero comparison)
    auto reference = y_model.norm();
    EXPECT_GT(reference, 0.0);

    // the difference y_problem - y_model
    y_problem.add(-1.0, y_model);

    // the two actions must agree up to round-off
    EXPECT_NEAR(y_problem.norm(), 0.0, 1.0e-14 * reference);

    // all done
    return;
}


TEST(Problems, Linear_First_Order_Problem)
{
    // create petsc environment
    auto environment = mito::linalg::petsc::environment();

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

    // the semi-discrete model
    auto semi_discrete_model = mito::fem::semi_discrete_model(function_space, model);

    // the linear first order problem
    auto problem = mito::fem::linear_first_order_problem(semi_discrete_model);

    // the problem type
    using problem_t = decltype(problem);

    // the problem satisfies the concept the time steppers rely on
    static_assert(mito::timesteppers::linear_first_order_problem_c<problem_t, backend_t>);
    // the heat diffusion problem has time invariant operators and a constant load
    static_assert(problem_t::time_invariant);
    static_assert(problem_t::constant_load);

    // the problem has as many equations as the semi-discrete model
    auto n_equations = problem.n_equations();
    EXPECT_EQ(n_equations, semi_discrete_model.n_equations());
    EXPECT_GT(n_equations, 0);

    // assemble the load through the problem
    auto load_problem = mito::linalg::petsc::vector("load_problem", n_equations);
    problem.assemble_load<backend_t>(load_problem);

    // assemble the load through the semi-discrete model
    auto load_model = mito::linalg::petsc::vector("load_model", n_equations);
    semi_discrete_model.set_vector_values<model_t::archetype_type::Load, backend_t>(load_model);
    load_model.assemble();

    // the two loads must agree
    auto load_reference = load_model.norm();
    EXPECT_GT(load_reference, 0.0);
    load_problem.add(-1.0, load_model);
    EXPECT_NEAR(load_problem.norm(), 0.0, 1.0e-14 * load_reference);

    // assemble the stiffness through the problem
    auto stiffness_problem = mito::linalg::petsc::matrix("stiffness_problem", n_equations);
    problem.set_pattern<backend_t>(stiffness_problem);
    problem.assemble_stiffness<backend_t>(stiffness_problem);

    // assemble the stiffness through the semi-discrete model
    auto stiffness_model = mito::linalg::petsc::matrix("stiffness_model", n_equations);
    semi_discrete_model.set_matrix_pattern<backend_t>(stiffness_model);
    semi_discrete_model.set_matrix_values<model_t::archetype_type::Stiffness, backend_t>(
        stiffness_model);
    stiffness_model.assemble();

    // the two stiffness matrices must act identically (the load is a convenient nonzero vector)
    expect_same_action(stiffness_problem, stiffness_model, load_model, n_equations);

    // assemble the mass through the problem
    auto mass_problem = mito::linalg::petsc::matrix("mass_problem", n_equations);
    problem.set_pattern<backend_t>(mass_problem);
    problem.assemble_mass<backend_t>(mass_problem);

    // assemble the capacitance through the semi-discrete model
    auto capacitance_model = mito::linalg::petsc::matrix("capacitance_model", n_equations);
    semi_discrete_model.set_matrix_pattern<backend_t>(capacitance_model);
    semi_discrete_model.set_matrix_values<model_t::archetype_type::Capacitance, backend_t>(
        capacitance_model);
    capacitance_model.assemble();

    // the two mass matrices must act identically
    expect_same_action(mass_problem, capacitance_model, load_model, n_equations);

    // all done
    return;
}

// end of file