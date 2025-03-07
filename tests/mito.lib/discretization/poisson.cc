// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the type of node
using node_t = cell_t::node_type;
// the type of simplex
using simplex_t = cell_t::simplex_type;
// Gauss quadrature on triangles with degree of exactness 1
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, simplex_t, 1>;
// TOFIX: the correct dimension of the parametric space
// // the dimension of the parametric space
// static constexpr int parametricDim = mito::manifolds::parametric_dim<simplex_t>();
// // the type of parametric coordinates
// using parametric_point_t = mito::manifolds::parametric_point_t<parametricDim>;
// the type of parametric coordinates
using parametric_point_t = mito::manifolds::parametric_point_t<2>;

// the function extracting the x component of a 2D vector
constexpr auto x = mito::functions::component<coordinates_t, 0>;
// the function extracting the y component of a 2D vector
constexpr auto y = mito::functions::component<coordinates_t, 1>;

// the function extracting the 0 component of a parametric point
constexpr auto xi_0 = mito::functions::component<coordinates_t, 0>;
// the function extracting the y component of a 2D vector
constexpr auto xi_1 = mito::functions::component<coordinates_t, 1>;

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

#if 0
    // TOFIX: this does not work as the nodes are copied into new nodes to construct the boundary
    // cells, therefore the set difference is not aware of the node equality. Therefore we need to
    // back up to the spatial search implementation

    // get all the nodes in the mesh
    std::set<node_t> nodes;
    mito::mesh::get_nodes(mesh, nodes);
    channel << "Number of total cells: " << std::size(mesh.cells()) << journal::endl;
    channel << "Number of total nodes: " << std::size(nodes) << journal::endl;
    for (const auto & node : nodes) {
        // print the coordinates of the node
        channel << coord_system.coordinates(node->point()) << journal::endl;
    }

    // get all the nodes on the mesh boundary
    auto boundary_mesh = mito::mesh::boundary(mesh);
    std::set<node_t> boundary_nodes;
    mito::mesh::get_nodes(boundary_mesh, boundary_nodes);
    channel << "Number of boundary cells: " << std::size(boundary_mesh.cells()) << journal::endl;
    channel << "Number of boundary nodes: " << std::size(boundary_nodes) << journal::endl;
    for (const auto & node : boundary_nodes) {
        // node.vertex()
        // print the coordinates of the node
        channel << coord_system.coordinates(node->point()) << journal::endl;
    }

    // get all the interior nodes as the difference between all the nodes and the boundary nodes
    std::set<node_t> interior_nodes;
    std::set_difference(
        nodes.begin(), nodes.end(), boundary_nodes.begin(), boundary_nodes.end(),
        std::inserter(interior_nodes, interior_nodes.begin()));
    channel << "Number of interior nodes: " << std::size(interior_nodes) << journal::endl;

    // populate the equation map (from node to equation, one equations per node)
    std::map<node_t, int> equation_map;
    int equation = 0;

    // loop on all the nodes of the cell
    for (const auto & node : interior_nodes) {
        // check if the node is already in the equation map
        if (equation_map.find(node) == equation_map.end()) {
            // add the node to the equation map
            equation_map[node] = equation;
            // increment the equation number
            equation++;
        }
    }
#else
    std::set<node_t> nodes;
    mito::mesh::get_nodes(mesh, nodes);

    // populate the equation map (from node to equation, one equations per node)
    std::map<node_t, int> equation_map;
    int equation = 0;

    // loop on all the nodes of the cell
    for (const auto & node : nodes) {
        // get the coordinates of the node
        auto coord = coord_system.coordinates(node->point());
        // if the node is on the boundary
        if (coord[0] == 0.0 || coord[0] == 1.0 || coord[1] == 0.0 || coord[1] == 1.0) {
            // check if the node is already in the equation map
            if (equation_map.find(node) == equation_map.end()) {
                // add the node to the equation map with a -1 indicating that the node is on the
                // boundary
                equation_map[node] = -1;
            }
        }

        // check if the node is already in the equation map
        if (equation_map.find(node) == equation_map.end()) {
            // add the node to the equation map
            equation_map[node] = equation;
            // increment the equation number
            equation++;
        }
    }

    // the number of equations
    int N_equations = equation;
#endif
    channel << "Number of equations: " << equation << journal::endl;

    // create a linear system of equations (PETSc Krylov solver)
    auto solver = mito::solvers::petsc::ksp("mysolver");
    solver.initialize(N_equations);
    solver.set_options("-ksp_monitor -ksp_converged_reason");

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // instantiate the quadrature rule
    constexpr auto quadrature_rule = quadrature_rule_t();
    static_assert(quadrature_rule_t::npoints == 1);

    // the {area} of the mesh
    auto area = 0.0;

    // the right hand side
    auto f = 2.0 * std::numbers::pi * std::numbers::pi * mito::functions::sin(std::numbers::pi * x)
           * mito::functions::sin(std::numbers::pi * y);
    // channel << "Right hand side: " << f(coordinates_t{ 0.5, 0.5 }) << journal::endl;

    // loop on all the cells of the mesh
    for (const auto & cell : manifold.elements()) {

        // the nodes of the cell
        const auto & nodes = cell.nodes();

        // the origin of the coordinate system
        auto origin = coordinates_t{};

        // the coordinates of the nodes of the triangle
        auto x_0 = coord_system.coordinates(nodes[0]->point()) - origin;
        auto x_1 = coord_system.coordinates(nodes[1]->point()) - origin;
        auto x_2 = coord_system.coordinates(nodes[2]->point()) - origin;

        // linear shape functions on the triangle
        constexpr auto phi_0 = mito::fields::field(xi_0);
        constexpr auto phi_1 = mito::fields::field(xi_1);
        constexpr auto phi_2 = 1.0 - phi_0 - phi_1;

        // the isoparametric mapping from the barycentric coordinates to the actual coordinates
        // on the cell {cell}
        auto x_cell = x_0 * phi_0 + x_1 * phi_1 + x_2 * phi_2;

        // the derivative of the coordinates with respect to the barycentric coordinates
        auto J = mito::fields::gradient(x_cell);

        // the barycentric coordinates of the only quadrature point of the quadrature rule
        constexpr auto xi =
            coordinates_t{ quadrature_rule.point(0)[0], quadrature_rule.point(0)[1] };

        // report (print the barycenter of the mesh cell)
        // channel << x_cell(xi) << journal::endl;

        // add up the area of the cell
        area += 1.0 / 2.0 * mito::tensor::determinant(J(xi));

        // evaluate the shape functions at {xi}
        using scalar_type = mito::tensor::scalar_t;
        std::array<scalar_type, 3> phi;
        phi[0] = phi_0(xi);
        phi[1] = phi_1(xi);
        phi[2] = phi_2(xi);

        // evaluate the gradients of the shape functions at {xi}
        using vector_type = mito::tensor::vector_t<2>;
        std::array<vector_type, 3> dphi;
        dphi[0] = mito::fields::gradient(phi_0)(xi) * mito::tensor::inverse(J(xi));
        dphi[1] = mito::fields::gradient(phi_1)(xi) * mito::tensor::inverse(J(xi));
        dphi[2] = mito::fields::gradient(phi_2)(xi) * mito::tensor::inverse(J(xi));

        auto factor = quadrature_rule.weight(0) * manifold.volume(cell);

        // the coordinates of the quadrature point
        auto coord = manifold.parametrization(cell, quadrature_rule.point(0));

        // populate the linear system of equations
        int a = 0;
        for (const auto & node_a : nodes) {
            int b = 0;
            // get the equation number of {node_a}
            int eq_a = equation_map.at(node_a);
            assert(eq_a < N_equations);
            if (eq_a == -1) {
                // skip boundary nodes
                continue;
            }
            for (const auto & node_b : nodes) {
                // get the equation number of {node_b}
                int eq_b = equation_map.at(node_b);
                assert(eq_b < N_equations);
                if (eq_b == -1) {
                    // skip boundary nodes
                    continue;
                }
                auto entry = factor * (dphi[a] * dphi[b]);

                solver.add_matrix_value(eq_a, eq_b, entry);

                ++b;
            }

            solver.add_rhs_value(eq_a, factor * f(coord) * phi[a]);

            ++a;
        }
    }

    // check that the area of the unit square is 1.0
    EXPECT_DOUBLE_EQ(1.0, area);

    solver.solve();

    // read the solution
    auto u = std::vector<double>(N_equations);
    solver.get_solution(u);

    // the numerical solution nodal field on the mesh
    auto solution =
        mito::discretization::nodal_field<mito::tensor::vector_t<1>>(mesh, "numerical solution");
    // fill information in nodal field
    for (auto & [node, value] : solution) {
        // get the equation number of {node}
        int eq = equation_map.at(node);
        if (eq != -1) {
            // read the solution at {eq}
            value[0] = u[eq];
        }
    }

    // the forcing term nodal field on the mesh
    auto forcing =
        mito::discretization::nodal_field<mito::tensor::vector_t<1>>(mesh, "forcing term");
    // fill information in nodal field
    for (auto & [node, value] : forcing) {
        // get the position of {node}
        auto coord = coord_system.coordinates(node->point());
        // evaluate the forcing at {coord}
        value[0] = f(coord);
    }

    // the exact solution nodal field on the mesh
    auto exact_solution =
        mito::discretization::nodal_field<mito::tensor::vector_t<1>>(mesh, "exact solution");
    auto u_ex =
        mito::functions::sin(std::numbers::pi * x) * mito::functions::sin(std::numbers::pi * y);
    // fill information in nodal field
    for (auto & [node, value] : exact_solution) {
        // get the position of {node}
        auto coord = coord_system.coordinates(node->point());
        // evaluate the forcing at {coord}
        value[0] = u_ex(coord);
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

    // compute the error
    auto error = 0.0;
    // loop on all the nodes of the cell
    for (const auto & node : nodes) {
        // get the exact solution at {coord}
        auto u_exact = exact_solution(node)[0];
        // get the numerical solution at {coord}
        auto u_numerical = solution(node)[0];
        // get the error
        error += (u_exact - u_numerical) * (u_exact - u_numerical);
    }
    error = std::sqrt(error);

    // report
    channel << "l2 error: " << error << journal::endl;

    // check that the l2 error is reasonable
    EXPECT_TRUE(error < 0.1);

    // TODO: add norm calculation for convergence study

    // finalize the solver
    solver.finalize();
}

// end of file
