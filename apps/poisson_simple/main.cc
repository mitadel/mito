// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito.h>
#include <cstdlib>     // for std::atoi
#include <iostream>    // for std::cerr

// cartesian coordinates in 1D
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;

// segments in 1D
using segment_t = mito::geometry::segment_t<1>;
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

// the x scalar field in 1D
constexpr auto x = mito::functions::component<coordinates_t, 0>;


int
main(int argc, char * argv[])
{
    // parse command-line argument for N, the number of segments
    int N = 50;    // default
    if (argc >= 2) {
        int val = std::atoi(argv[1]);
        if (val > 0) {
            N = val;
        } else {
            std::cerr << "Invalid value for N, using default N = 50.\n";
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " N\n";
        std::cerr << "No N given, using default N = 50.\n";
    }

    // initialize PETSc
    mito::petsc::initialize();

    // make a channel
    journal::info_t channel("apps.poisson_simple");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // create a mesh of segments
    auto mesh = mito::mesh::mesh<segment_t>();

    // build nodes
    const double L = 10.0;     // length of the domain
    const double h = L / N;    // length of each segment

    auto prev = mito::geometry::node(coord_system, { 0.0 });

    for (int i = 1; i <= N; i++) {
        auto node = mito::geometry::node(coord_system, { static_cast<double>(i) * h });
        mesh.insert({ prev, node });
        prev = node;
    }

    channel << "Created mesh with " << mesh.nCells() << " segments" << journal::endl;

    // create the body manifold (non-embedded: 1D in 1D)
    // the manifold factory creates a manifold object with the appropriate volume form
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // deduce the finite element type from the manifold
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

    // the right hand side: f = sin(pi * x)
    auto f = mito::functions::sin(std::numbers::pi * x);

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

    // the exact solution field: u(x) = (1/pi²)sin(pi * x)
    auto u_ex =
        (1.0 / (std::numbers::pi * std::numbers::pi)) * mito::functions::sin(std::numbers::pi * x);

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
    auto writer = mito::io::vtk::field_writer("poisson_simple", mesh, coord_system);
    writer.record(solution_mesh_field);
    writer.record(exact_solution);
    writer.record(forcing);
    writer.write();

    channel << "VTK file written: poisson_simple.vtu" << journal::endl;
#endif

    // finalize PETSc
    mito::petsc::finalize();

    // all done
    return 0;
}

// end of file
