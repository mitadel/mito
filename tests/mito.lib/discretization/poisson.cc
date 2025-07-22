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


// the function extracting the x component of a 2D vector
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the function extracting the y component of a 2D vector
constexpr auto y = mito::functions::component<coordinates_t, 1>;


TEST(Fem, PoissonSquare)
{
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
    auto function_space = mito::discretization::function_space<2>(manifold, constraints);

    // the discrete system
    auto discrete_system = mito::discretization::discrete_system(function_space);
    const auto & equation_map = discrete_system.equation_map();
    int N_equations = discrete_system.n_equations();

    // TODO: blocks should be signed up with the discrete system. Then the loop on the elements
    // should be handled by the system

    // QUESTION: perhaps add the possibility to have a label for the blocks
    // a grad-grad matrix block
    auto fem_grad_grad_block =
        mito::discretization::blocks::matrix_block<quadrature_rule_t>(manifold);

    // a rhs block
    auto fem_rhs_block = mito::discretization::blocks::vector_block<quadrature_rule_t>(manifold);

    // create a linear system of equations (PETSc Krylov solver)
    auto solver = mito::solvers::petsc::ksp("mysolver");
    solver.initialize(N_equations);
    solver.set_options("-ksp_type preonly -pc_type cholesky");

    // the right hand side
    auto f = 2.0 * std::numbers::pi * std::numbers::pi * mito::functions::sin(std::numbers::pi * x)
           * mito::functions::sin(std::numbers::pi * y);
    // channel << "Right hand side: " << f(coordinates_t{ 0.5, 0.5 }) << journal::endl;

    // the number of nodes per element
    constexpr int n_nodes =
        mito::utilities::base_type<decltype(function_space)>::element_type::n_nodes;

    // loop on all the cells of the mesh
    for (const auto & element : function_space.elements()) {

        // assemble the elementary stiffness matrix
        auto elementary_stiffness_matrix = fem_grad_grad_block.compute(element);

        // populate the linear system of equations
        mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
            // get the a-th discretization node of the element
            const auto & node_a = element.connectivity()[a];
            // get the equation number of {node_a}
            int eq_a = equation_map.at(node_a);
            assert(eq_a < N_equations);
            if (eq_a != -1) {
                mito::tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                    // get the b-th discretization node of the element
                    const auto & node_b = element.connectivity()[b];
                    // get the equation number of {node_b}
                    int eq_b = equation_map.at(node_b);
                    assert(eq_b < N_equations);
                    // non boundary nodes
                    if (eq_b != -1) {
                        // assemble the value in the stiffness matrix
                        solver.add_matrix_value(eq_a, eq_b, elementary_stiffness_matrix[{ a, b }]);
                    }
                });
            }
        });

        // assemble the elementary right hand side
        auto elementary_rhs = fem_rhs_block.compute(element, f);

        // populate the right hand side
        mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
            // get the a-th discretization node of the element
            const auto & node_a = element.connectivity()[a];
            // get the equation number of {node_a}
            int eq_a = equation_map.at(node_a);
            assert(eq_a < N_equations);
            // non boundary nodes
            if (eq_a != -1) {
                // assemble the value in the right hand side
                solver.add_rhs_value(eq_a, elementary_rhs[{ a }]);
            }
        });
    }

    solver.solve();

    // read the solution
    auto u = std::vector<double>(N_equations);
    solver.get_solution(u);

    // finalize the solver
    solver.finalize();

    // TOFIX: the solution should be assembled by the function space, which is aware of the
    // constraints and can populate the constrained nodes appropriately
    // the numerical solution nodal field on the mesh
    auto solution = mito::discretization::nodal_field<scalar_t>(mesh, "numerical solution");
    // get the node map from the function space
    auto node_map = function_space.node_map();
    // fill information in nodal field
    for (auto & [node, value] : solution) {
        auto discretization_node = node_map.at(node);
        // get the equation number of {node}
        int eq = equation_map.at(discretization_node);
        if (eq != -1) {
            // read the solution at {eq}
            value = u[eq];
        }
    }

    // the forcing term nodal field on the mesh
    auto forcing = mito::discretization::nodal_field<scalar_t>(mesh, "forcing term");
    // fill information in nodal field
    for (auto & [node, value] : forcing) {
        // get the position of {node}
        auto coord = coord_system.coordinates(node->point());
        // evaluate the forcing at {coord}
        value = f(coord);
    }

    // the exact solution nodal field on the mesh
    auto exact_solution = mito::discretization::nodal_field<scalar_t>(mesh, "exact solution");
    auto u_ex = mito::fields::field(
        mito::functions::sin(std::numbers::pi * x) * mito::functions::sin(std::numbers::pi * y));
    // fill information in nodal field
    for (auto & [node, value] : exact_solution) {
        // get the position of {node}
        auto coord = coord_system.coordinates(node->point());
        // evaluate the forcing at {coord}
        value = u_ex(coord);
    }

#ifdef WITH_VTK
    // write mesh to vtk file
    auto writer = mito::io::vtk::field_writer("poisson_square", mesh, coord_system);
    // sign {forcing} up with the writer
    writer.record(forcing);
    // sign {solution} up with the writer
    writer.record(solution);
    // sign {exact_solution} up with the writer
    writer.record(exact_solution);
    // write output file
    writer.write();
#endif

    // instantiate the quadrature rule
    auto quadrature_rule = quadrature_rule_t();

    // compute the L2 error
    auto error_L2 = 0.0;
    // loop on all the cells of the mesh
    for (const auto & element : function_space.elements()) {
        // get the corresponding cell
        const auto & cell = element.geometric_simplex();
        // volume of the cell
        auto volume = manifold.volume(cell);
        // loop on the quadrature points
        for (int q = 0; q < quadrature_rule_t::npoints; ++q) {
            // the barycentric coordinates of the quadrature point
            /*constexpr*/ auto xi = quadrature_rule.point(q);
            // the coordinates of the quadrature point
            auto coord = manifold.parametrization(cell, quadrature_rule.point(q));
            // get the exact solution at {coord}
            auto u_exact = u_ex(coord);
            // assemble the numerical solution at {coord}
            auto u_numerical = 0.0;
            // loop on all the shape functions
            mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                // get the a-th discretization node of the element
                const auto & node_a = element.connectivity()[a];
                // evaluate the a-th shape function at {xi}
                auto phi_a = element.shape<a>()(xi);
                // get the equation number of {node_a}
                int eq = equation_map.at(node_a);
                if (eq != -1) {
                    // get the numerical solution at {coord}
                    u_numerical += u[eq] * phi_a;
                }
            });
            // get the error
            error_L2 += (u_exact - u_numerical) * (u_exact - u_numerical)
                      * quadrature_rule.weight(q) * volume;
        }
    }
    error_L2 = std::sqrt(error_L2);

    // report
    channel << "L2 error: " << error_L2 << journal::endl;

    // check that the l2 error is reasonable
    EXPECT_TRUE(error_L2 < 0.02);

    // compute the H1 error
    auto error_H1 = 0.0;
    // loop on all the cells of the mesh
    for (const auto & element : function_space.elements()) {
        // get the corresponding cell
        const auto & cell = element.geometric_simplex();
        // volume of the cell
        auto volume = manifold.volume(cell);
        for (int q = 0; q < quadrature_rule_t::npoints; ++q) {
            // the barycentric coordinates of the quadrature point
            auto xi = quadrature_rule.point(q);
            // the coordinates of the quadrature point
            auto coord = manifold.parametrization(cell, xi);
            // exact solution gradient at {coord}
            auto grad_u_exact = mito::fields::gradient(u_ex)(coord);
            // assemble the numerical solution gradient at {coord}
            auto grad_u_numerical = mito::tensor::vector_t<2>{ 0.0, 0.0 };
            // loop on all the shape functions
            mito::tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                // get the a-th discretization node of the element
                const auto & node_a = element.connectivity()[a];
                // evaluate the a-th shape function gradient at {xi}
                auto dphi_a = element.gradient<a>()(xi);
                // get the equation number of {node_a}
                int eq = equation_map.at(node_a);
                if (eq != -1) {
                    // get the numerical solution gradient at {coord}
                    grad_u_numerical += u[eq] * dphi_a;
                }
            });
            // get the error
            auto diff = grad_u_exact - grad_u_numerical;
            error_H1 += mito::tensor::dot(diff, diff) * quadrature_rule.weight(q) * volume;
        }
    }
    error_H1 = std::sqrt(error_H1);

    // report
    channel << "H1 error: " << error_H1 << journal::endl;

    // check that the h1 error is reasonable
    EXPECT_TRUE(error_H1 < 0.02);
}

// end of file
