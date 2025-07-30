// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the scalar type
using scalar_t = mito::tensor::scalar_t;
// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the type of simplex
using simplex_t = cell_t::simplex_type;
// Gauss quadrature on triangles with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, simplex_t, 2>;

// the degree of the finite element
constexpr int degree = 2;
// typedef for a finite element
using finite_element_t = mito::discretization::isoparametric_simplex_t<degree, cell_t>;

// typedef for a linear system of equations
using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
// typedef for a matrix solver
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

// NOTE: component -> projection
// the function extracting the x component of a 2D vector
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the function extracting the y component of a 2D vector
constexpr auto y = mito::functions::component<coordinates_t, 1>;

// TODO: add unit tests for blocks individually


TEST(Fem, PoissonSquare)
{
    // initialize PETSc
    mito::petsc::initialize();

    // make a channel
    journal::info_t channel("tests.poisson_square");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a square in 2D
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, coord_system);

    // // TOFIX: tetra does not work with the current implementation
    // // do one tetra mesh refinement
    // const auto subdivisions = 1;
    // auto mesh = mito::mesh::tetra(original_mesh, coord_system, subdivisions);

    // the zero field
    auto zero = mito::fields::field(mito::functions::zero<coordinates_t>);

    // get all the nodes on the mesh boundary
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // the function space (linear elements on the manifold)
    // TOFIX: function space should be template with respect to the finite element type
    auto function_space = mito::discretization::function_space<degree>(manifold, constraints);

    // TODO: all top level instances have names. Name should be the first argument. Then we can use
    // names in the configuration file and in the hdf5 file. Check libuuid vs. leading namestring.
    //
    // the discrete system
    auto discrete_system =
        mito::discretization::discrete_system<linear_system_t>(function_space, "mysystem");

    // instantiate a linear solver for the discrete system
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);
    solver.create();

    // QUESTION: do we need a method to set the options for the solver? Can this go in the
    // constructor or in the create method?
    // in the python layer, this will go in the configuration file
    solver.set_options("-ksp_type preonly -pc_type cholesky");

    // a grad-grad matrix block
    auto fem_lhs_block =
        mito::discretization::blocks::grad_grad_block<finite_element_t, quadrature_rule_t>();

    // the right hand side
    auto f = mito::fields::field(
        2.0 * std::numbers::pi * std::numbers::pi * mito::functions::sin(std::numbers::pi * x)
        * mito::functions::sin(std::numbers::pi * y));
    // channel << "Right hand side: " << f(coordinates_t{ 0.5, 0.5 }) << journal::endl;

    // a source term block
    auto fem_rhs_block =
        mito::discretization::blocks::source_term_block<finite_element_t, quadrature_rule_t>(f);

    // TODO:
    // // monolithic discretization matrix = [A, B; C, D]
    // auto weakform_lhs = fem_lhs_block;
    // auto weakform_rhs = fem_rhs_block;
    // auto weakform = weakform(weakform_lhs, weakform_rhs);
    // discrete_system.set_weak_form(weakform);

    // add the blocks to the discrete system
    discrete_system.add_lhs_block(fem_lhs_block);
    discrete_system.add_rhs_block(fem_rhs_block);

    // solve the linear system
    solver.solve();

    // free the solver
    solver.destroy();

    // the exact solution field
    auto u_ex = mito::fields::field(
        mito::functions::sin(std::numbers::pi * x) * mito::functions::sin(std::numbers::pi * y));

    // compute the L2 error
    auto error_L2 = discrete_system.compute_l2_error<quadrature_rule_t>(u_ex);

    // report
    channel << "L2 error: " << error_L2 << journal::endl;

    // check that the l2 error is reasonable
    EXPECT_TRUE(error_L2 < 0.02);

    // compute the H1 error
    auto error_H1 = discrete_system.compute_h1_error<quadrature_rule_t>(u_ex);

    // report
    channel << "H1 error: " << error_H1 << journal::endl;

    // check that the h1 error is reasonable
    EXPECT_TRUE(error_H1 < 0.02);

#ifdef WITH_VTK
    // the forcing term mesh field on the mesh (for visualization)
    auto forcing = mito::discretization::mesh_field(mesh, coord_system, f, "forcing term");
    // the exact solution mesh field on the mesh (for visualization)
    auto exact_solution =
        mito::discretization::mesh_field(mesh, coord_system, u_ex, "exact solution");
    // write mesh to vtk file
    auto writer = mito::io::vtk::field_writer("poisson_square_data", mesh, coord_system);
    // sign {forcing} up with the writer
    writer.record(forcing);
    // sign {exact_solution} up with the writer
    writer.record(exact_solution);
    // write output file
    writer.write();

    // get the solution field
    const auto & solution = discrete_system.solution();
    // write mesh to vtk file
    auto writer_solution =
        mito::io::vtk::field_writer("poisson_square_solution", function_space, coord_system);
    // sign {solution} up with the writer
    writer_solution.record(solution);
    // write output file
    writer_solution.write();
#endif

    // finalize PETSc
    mito::petsc::finalize();
}

// end of file
