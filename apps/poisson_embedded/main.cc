// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito.h>
#include <cstdlib>     // for std::atoi
#include <iostream>    // for std::cerr

// cartesian coordinates in 2D (for embedded segments)
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// segments embedded in 2D
using segment_t = mito::geometry::segment_t<2>;

// domain length (arc length along the diagonal)
constexpr double L = 10.0;
// the diagonal goes from (0,0) to (8, 6) so that its length is 10
// direction vector (normalized): (8/10, 6/10)
constexpr double dir_x = 0.8;
constexpr double dir_y = 0.6;

// first degree (linear) finite elements
constexpr int degree = 1;

// the reference simplex
using reference_simplex_t = mito::geometry::reference_segment_t;
// degree of exactness for the quadrature rule
constexpr int doe = 1;
// Gauss quadrature on segments with degree of exactness 1
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, doe>;

// typedef for a linear system of equations
using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
// typedef for a matrix solver
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

// the x and y scalar fields in 2D
constexpr auto x = mito::functions::component<coordinates_t, 0>;
constexpr auto y = mito::functions::component<coordinates_t, 1>;


int
main(int argc, char * argv[])
{
    // parse command-line argument for N, the number of segments
    int N = 100;    // default (same as the 1D case: 100 segments, 101 nodes)
    if (argc >= 2) {
        int val = std::atoi(argv[1]);
        if (val > 0) {
            N = val;
        } else {
            std::cerr << "Invalid value for N, using default N = 100.\n";
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " N\n";
        std::cerr << "No N given, using default N = 100.\n";
    }

    // initialize PETSc
    mito::petsc::initialize();

    // make a channel
    journal::info_t channel("apps.poisson_embedded");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // create a mesh of embedded segments along a diagonal
    auto mesh = mito::mesh::mesh<segment_t>();

    // segment length in arc length
    const double h = L / N;

    // build the mesh along the diagonal
    auto prev = mito::geometry::node(coord_system, { 0.0, 0.0 });

    for (int i = 1; i <= N; i++) {
        double s = static_cast<double>(i) * h;    // arc length parameter
        auto node = mito::geometry::node(coord_system, { s * dir_x, s * dir_y });
        mesh.insert({ prev, node });
        prev = node;
    }

    channel << "Created embedded mesh with " << mesh.nCells() << " segments along diagonal"
            << journal::endl;
    channel << "Diagonal endpoints: (0, 0) to (" << L * dir_x << ", " << L * dir_y << ")"
            << journal::endl;

    // for a submanifold (1D curve in 2D), we need to provide the normal field
    // the tangent to the diagonal is (dir_x, dir_y), so the normal is (-dir_y, dir_x)
    constexpr double normal_x = -dir_y;
    constexpr double normal_y = dir_x;
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ normal_x, normal_y });

    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    using finite_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(manifold)>;

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

    // the forcing function: f = sin(pi * s) where s is the arc length
    // for points on the diagonal from origin: s = sqrt(x² + y²)
    // so f(x, y) = sin(pi * sqrt(x² + y²))
    auto f = mito::functions::sin(std::numbers::pi * mito::functions::sqrt(x * x + y * y));

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

    // the exact solution: u(s) = (1/pi²) * sin(pi * s) where s = sqrt(x² + y²)
    auto u_ex = (1.0 / (std::numbers::pi * std::numbers::pi))
              * mito::functions::sin(std::numbers::pi * mito::functions::sqrt(x * x + y * y));

    // compute the L2 error
    // auto error_L2 = discrete_system.compute_l2_error<quadrature_rule_t>(u_ex);
    // report
    // channel << "L2 error: " << error_L2 << journal::endl;

    // compute the H1 error
    // auto error_H1 = discrete_system.compute_h1_error<quadrature_rule_t>(u_ex);
    // report
    // channel << "H1 error: " << error_H1 << journal::endl;

#ifdef WITH_VTK
    // get the solution field
    const auto & solution = discrete_system.solution();

    // convert solution to mesh field for easier visualization
    // get the node map from function space to mesh nodes
    const auto & node_map = function_space.node_map();
    auto solution_mesh_field =
        mito::discrete::mesh_field<mito::tensor::scalar_t>(mesh, "numerical solution");

    // populate mesh field with solution values
    for (const auto & [mesh_node, disc_node] : node_map) {
        solution_mesh_field(mesh_node) = solution(disc_node);
    }

    // write all fields to the same file for comparison
    auto exact_solution = mito::discrete::mesh_field(mesh, coord_system, u_ex, "exact solution");
    auto forcing = mito::discrete::mesh_field(mesh, coord_system, f, "forcing term");
    auto writer = mito::io::vtk::field_writer("poisson_embedded", mesh, coord_system);
    writer.record(solution_mesh_field);
    writer.record(exact_solution);
    writer.record(forcing);
    writer.write();

    channel << "VTK file written: poisson_embedded.vtu" << journal::endl;
#endif

    // finalize PETSc
    mito::petsc::finalize();

    // all done
    return 0;
}

// end of file
