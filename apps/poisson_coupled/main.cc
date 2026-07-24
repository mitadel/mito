// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <mito.h>
#include <cmath>
#include <numbers>
#include <vector>
#include <cstdlib>     // for std::atoi
#include <iostream>    // for std::cerr


// 2D Cartesian coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;

// cell types
using triangle_t = mito::geometry::triangle_t<2>;
using segment_t = mito::geometry::segment_t<2>;
using node_t = mito::geometry::node_t<2>;

// linear elements on both domains
constexpr int degree = 1;

// quadrature rules
using bulk_reference_simplex_t = mito::geometry::reference_triangle_t;
using interface_reference_simplex_t = mito::geometry::reference_segment_t;
constexpr int doe = 1;
using bulk_quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, bulk_reference_simplex_t, doe>;
using interface_quadrature_rule_t = mito::quadrature::quadrature_rule_t<
    mito::quadrature::GAUSS, interface_reference_simplex_t, doe>;

// PETSc linear algebra
using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;


// mesh helpers (same mesh as getting_started)
auto
create_interface_mesh(const std::vector<node_t> & nodes) -> mito::mesh::mesh_t<segment_t>
{
    auto result = mito::mesh::mesh<segment_t>();
    for (size_t i = 0; i < nodes.size() - 1; ++i) {
        result.insert({ nodes[i], nodes[i + 1] });
    }
    return result;
}

std::vector<node_t>
create_triangle_row(
    mito::mesh::mesh_t<triangle_t> & mesh, coord_system_t & coord_system,
    const std::vector<node_t> & bottom_nodes, double l, double h, double top_y, bool pointing_up)
{
    const int N = static_cast<int>(bottom_nodes.size()) - 1;
    std::vector<node_t> top_nodes;

    if (pointing_up) {
        auto base_x = coord_system.coordinates(bottom_nodes[0]->point())[0];
        for (int i = 0; i < N; ++i) {
            top_nodes.push_back(
                mito::geometry::node(coord_system, { base_x + (i + 0.5) * l, top_y }));
        }
        for (int i = 0; i < N; ++i) {
            mesh.insert({ bottom_nodes[i], bottom_nodes[i + 1], top_nodes[i] });
            if (i < N - 1) {
                mesh.insert({ bottom_nodes[i + 1], top_nodes[i + 1], top_nodes[i] });
            }
        }
    } else {
        auto base_x = coord_system.coordinates(bottom_nodes[0]->point())[0];
        for (int i = 0; i <= N + 1; ++i) {
            top_nodes.push_back(
                mito::geometry::node(coord_system, { base_x + (i - 0.5) * l, top_y }));
        }
        for (int i = 0; i <= N; ++i) {
            mesh.insert({ bottom_nodes[i], top_nodes[i + 1], top_nodes[i] });
            if (i < N) {
                mesh.insert({ bottom_nodes[i], bottom_nodes[i + 1], top_nodes[i + 1] });
            }
        }
    }

    return top_nodes;
}

std::vector<node_t>
create_base_nodes(coord_system_t & coord_system, int N, double l, double x_start, double y)
{
    std::vector<node_t> nodes;
    for (int i = 0; i <= N; ++i) {
        nodes.push_back(mito::geometry::node(coord_system, { x_start + double(i) * l, y }));
    }
    return nodes;
}

// main

int
main(int argc, char * argv[])
{
    // command-line parameters
    int num_bottom_segments = 30;
    int num_rows = 12;
    if (argc >= 2) {
        int val = std::atoi(argv[1]);
        if (val > 0)
            num_bottom_segments = val;
        else
            std::cerr << "Invalid num_bottom_segments, using default = 30.\n";
    }
    if (argc >= 3) {
        int val = std::atoi(argv[2]);
        if (val > 0 && val % 2 == 0)
            num_rows = val;
        else
            std::cerr << "Invalid num_rows (must be even and positive), using default = 12.\n";
    }

    journal::info_t channel("apps.poisson_coupled");

    // initialize PETSc
    mito::petsc::initialize();

    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    const double L = 10.0;
    const double l = L / num_bottom_segments;
    const double h = std::sqrt(3.0) / 2.0 * l;
    const double y_start = -h * num_rows / 2.0;

    // build bulk triangle mesh
    auto mesh = mito::mesh::mesh<triangle_t>();

    auto current_nodes = create_base_nodes(coord_system, num_bottom_segments, l, 0.0, y_start);

    double current_y = y_start;
    bool pointing_up = true;
    std::vector<node_t> centerline_nodes;
    const int centerline_row = num_rows / 2;

    for (int row = 0; row < num_rows; ++row) {
        current_y += h;
        current_nodes =
            create_triangle_row(mesh, coord_system, current_nodes, l, h, current_y, pointing_up);
        if (row == centerline_row - 1) {
            centerline_nodes = current_nodes;
        }
        pointing_up = !pointing_up;
    }

    channel << "Created 2D mesh with " << mesh.nCells() << " triangles" << journal::endl;

    // build interface (centerline) segment mesh
    auto interface_mesh = create_interface_mesh(centerline_nodes);
    channel << "Interface mesh at centerline: " << interface_mesh.nCells() << " segments"
            << journal::endl;

    // manifolds
    // non-embedded 2D manifold for the bulk triangles
    auto bulk_manifold = mito::manifolds::manifold(mesh, coord_system);

    // embedded 1D submanifold for the horizontal centerline (normal = e_y)
    auto normal = mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, 1.0 });
    auto interface_manifold = mito::manifolds::submanifold(interface_mesh, coord_system, normal);

    // element types
    // P1 triangle in 2D
    using bulk_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(bulk_manifold)>;
    // P1 segment embedded in 2D
    using interface_element_t =
        mito::fem::isoparametric_simplex_t<degree, decltype(interface_manifold)>;

    // constraints (homogeneous Dirichlet on all external boundaries)
    auto zero = mito::functions::zero<coordinates_t>;

    // bulk: all outer edges of the triangle mesh
    auto bulk_boundary = mito::mesh::boundary(mesh);
    auto bulk_constraints = mito::constraints::dirichlet_bc(bulk_boundary, zero);

    // interface: the two endpoints of the centerline
    auto interface_boundary = mito::mesh::boundary(interface_mesh);
    auto interface_constraints = mito::constraints::dirichlet_bc(interface_boundary, zero);

    // function spaces
    // bulk function space; populates the discretization node map for all triangle vertices
    auto fs_bulk = mito::fem::function_space<bulk_element_t>(bulk_manifold, bulk_constraints);

    // interface function space; pre-seeded with the bulk's node map so that centerline mesh nodes
    // reuse the same discretization_node_t objects already created for the bulk
    auto fs_interface = mito::fem::function_space<interface_element_t>(
        interface_manifold, interface_constraints, fs_bulk.node_map());

    channel << "Built bulk function space with " << fs_bulk.elements().size() << " elements"
            << journal::endl;
    channel << "Built interface function space with " << fs_interface.elements().size()
            << " elements" << journal::endl;

    // source terms
    constexpr auto x_coord = mito::functions::component<coordinates_t, 0>;
    constexpr auto y_coord = mito::functions::component<coordinates_t, 1>;

    // f_bulk = 2pi² sin(pi * x) cos(pi * y)  (RHS of -Δφ = f for φ = sin(pi * x)cos(pi * y))
    auto f_bulk = (2.0 * std::numbers::pi * std::numbers::pi)
                * mito::functions::sin(std::numbers::pi * x_coord)
                * mito::functions::cos(std::numbers::pi * y_coord);

    // no source term on the interface
    auto f_interface = mito::functions::zero<coordinates_t>;

    // weak forms
    // bulk: stiffness + load
    auto bulk_lhs_block =
        mito::fem::blocks::grad_grad_block<bulk_element_t, bulk_quadrature_rule_t>();
    auto bulk_rhs_block =
        mito::fem::blocks::source_term_block<bulk_element_t, bulk_quadrature_rule_t>(f_bulk);
    auto wf_bulk = mito::fem::weakform<bulk_element_t>();
    wf_bulk.add_block(bulk_lhs_block);
    wf_bulk.add_block(bulk_rhs_block);

    // interface: stiffness only (no source)
    auto interface_lhs_block =
        mito::fem::blocks::grad_grad_block<interface_element_t, interface_quadrature_rule_t>();
    auto wf_interface = mito::fem::weakform<interface_element_t>();
    wf_interface.add_block(interface_lhs_block);

    // coupled discrete system
    // physical parameters
    constexpr double kappa_SE = 1.0;     // bulk conductivity
    constexpr double w_kappa_m = 1.0;    // interface thickness * conductivity (w*kappa_m)

    auto discrete_system = mito::fem::discrete_system<linear_system_t>(
        "poisson_coupled", mito::fem::Contribution{ fs_bulk, wf_bulk, kappa_SE },
        mito::fem::Contribution{ fs_interface, wf_interface, -w_kappa_m });

    channel << "Coupled system: " << discrete_system.n_equations() << " free DOFs" << journal::endl;

    // solve
    channel << "Creating linear solver..." << journal::endl;
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);
    channel << "Setting options..." << journal::endl;
    solver.set_options("-ksp_type preonly -pc_type lu");
    channel << "Calling solve()..." << journal::endl;
    solver.solve();

    channel << "Solved coupled bulk-interface Poisson problem" << journal::endl;

    solver.destroy();

#ifdef WITH_VTK
    // write the numerical solution on the bulk (2D triangle) mesh
    auto solution_field = mito::discrete::mesh_field<mito::tensor::scalar_t>(mesh, "solution");
    const auto & node_map = fs_bulk.node_map();
    const auto & solution = discrete_system.solution();

    for (auto & [mesh_node, value] : solution_field) {
        auto it = node_map.find(mesh_node);
        if (it != node_map.end()) {
            // boundary nodes keep their default value of 0
            const auto & disc_node = it->second;
            // only write interior nodes
            value = solution(disc_node);
        }
    }

    auto writer = mito::io::vtk::field_writer("poisson_coupled", mesh, coord_system);
    writer.record(solution_field);
    writer.write();

    channel << "VTK file written: poisson_coupled.vtu" << journal::endl;
#endif

    mito::petsc::finalize();

    return 0;
}

// end of file
