// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// simplicial cells in 2D
using cell_t = mito::geometry::triangle_t<2>;
// second degree finite elements
constexpr int degree = 2;
// assemble the finite element type
using finite_element_t = mito::fem::isoparametric_simplex_t<degree, cell_t>;

// the reference simplex
using reference_simplex_t = mito::geometry::reference_triangle_t;
// degree of exactness for the quadrature rule
constexpr int doe = 2;
// Gauss quadrature on triangles with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, doe>;

// typedef for a linear system of equations
using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
// typedef for a matrix solver
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

// the x scalar field in 2D
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the y scalar field in 2D
constexpr auto y = mito::functions::component<coordinates_t, 1>;


int
main()
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

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // get the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the zero field
    auto zero = mito::functions::zero<coordinates_t>;

    // set homogeneous Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // the function space (linear elements on the manifold)
    auto function_space = mito::fem::function_space<finite_element_t>(manifold, constraints);

    // a grad-grad matrix block
    auto fem_lhs_block = mito::fem::blocks::grad_grad_block<finite_element_t, quadrature_rule_t>();

    // the right hand side
    auto f = 2.0 * std::numbers::pi * std::numbers::pi * mito::functions::sin(std::numbers::pi * x)
           * mito::functions::sin(std::numbers::pi * y);
    // channel << "Right hand side: " << f(coordinates_t{ 0.5, 0.5 }) << journal::endl;

    // a source term block
    auto fem_rhs_block =
        mito::fem::blocks::source_term_block<finite_element_t, quadrature_rule_t>(f);

    // create the weak form and populate it with the blocks
    auto weakform = mito::fem::weakform<finite_element_t>();
    weakform.add_block(fem_lhs_block);
    weakform.add_block(fem_rhs_block);

    // the discrete system
    auto discrete_system =
        mito::fem::discrete_system<linear_system_t>("mysystem", function_space, weakform);

    // instantiate a linear solver for the discrete system
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);

    // set options for the backend {petsc} matrix solver
    solver.set_options("-ksp_type preonly -pc_type cholesky");

    // solve the system
    solver.solve();

    // free the solver
    solver.destroy();

    // get the solution field
    const auto & solution = discrete_system.solution();

    // the exact solution field
    auto u_ex =
        mito::functions::sin(std::numbers::pi * x) * mito::functions::sin(std::numbers::pi * y);

    // compute the L2 error
    auto error_L2 = mito::fem::compute_l2_norm<quadrature_rule_t>(function_space, solution, u_ex);
    // report
    channel << "L2 error: " << error_L2 << journal::endl;

    // compute the H1 error
    auto error_H1 = mito::fem::compute_h1_norm<quadrature_rule_t>(function_space, solution, u_ex);
    // report
    channel << "H1 error: " << error_H1 << journal::endl;

#ifdef WITH_VTK
    // the forcing term mesh field on the mesh (for visualization)
    auto forcing = mito::discrete::mesh_field(mesh, coord_system, f, "forcing term");
    // the exact solution mesh field on the mesh (for visualization)
    auto exact_solution = mito::discrete::mesh_field(mesh, coord_system, u_ex, "exact solution");
    // write mesh to vtk file
    auto writer = mito::io::vtk::field_writer("poisson_square_data", mesh, coord_system);
    // sign {forcing} up with the writer
    writer.record(forcing);
    // sign {exact_solution} up with the writer
    writer.record(exact_solution);
    // write output file
    writer.write();

    // write mesh to vtk file
    auto writer_solution =
        mito::io::vtk::field_writer("poisson_square_solution", function_space, coord_system);
    // sign {solution} up with the writer
    writer_solution.record(solution.nodal_values(), "numerical solution");
    // write output file
    writer_solution.write();
#endif

    // finalize PETSc
    mito::petsc::finalize();
}

// end of file
