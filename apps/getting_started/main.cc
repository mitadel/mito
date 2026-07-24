// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <mito.h>
#include <cmath>
#include <vector>
#include <cstdlib>     // for std::atoi
#include <iostream>    // for std::cerr

// 2D Cartesian coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;

// cell types
using triangle_t = mito::geometry::triangle_t<2>;
using segment_t = mito::geometry::segment_t<2>;    // 1D segments embedded in 2D
using node_t = mito::geometry::node_t<2>;

// finite element degree
constexpr int degree = 1;

// reference simplices and quadrature rules for interface (segments) and bulk (triangles)
using reference_segment_t = mito::geometry::reference_segment_t;
using reference_triangle_t = mito::geometry::reference_triangle_t;
constexpr int doe = 1;    // degree of exactness
using quadrature_rule_segment_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_segment_t, doe>;
using quadrature_rule_triangle_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_triangle_t, doe>;

// linear system types (PETSc)
using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

// helper function to create interface mesh from consecutive nodes
auto
create_interface_mesh(const std::vector<node_t> & nodes) -> mito::mesh::mesh_t<segment_t>
{
    auto result = mito::mesh::mesh<segment_t>();

    // Create segments connecting consecutive nodes
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
    const int N = bottom_nodes.size() - 1;    // number of segments on bottom

    std::vector<node_t> top_nodes;

    if (pointing_up) {
        // upward row: N top nodes at x = bottom[0].x + 0.5, bottom[0].x + 1.5, ...
        auto base_x = coord_system.coordinates(bottom_nodes[0]->point())[0];
        for (int i = 0; i < N; ++i) {
            top_nodes.push_back(
                mito::geometry::node(coord_system, { base_x + (i + 0.5) * l, top_y }));
        }

        // insert triangles
        for (int i = 0; i < N; ++i) {
            // upward triangle: bottom[i], bottom[i+1], top[i]
            mesh.insert({ bottom_nodes[i], bottom_nodes[i + 1], top_nodes[i] });

            // downward triangle (connects two top nodes): bottom[i+1], top[i+1], top[i]
            if (i < N - 1) {
                mesh.insert({ bottom_nodes[i + 1], top_nodes[i + 1], top_nodes[i] });
            }
        }
    } else {
        // downward row: N+2 top nodes extending beyond bottom
        auto base_x = coord_system.coordinates(bottom_nodes[0]->point())[0];
        for (int i = 0; i <= N + 1; ++i) {
            top_nodes.push_back(
                mito::geometry::node(coord_system, { base_x + (i - 0.5) * l, top_y }));
        }

        // insert triangles
        for (int i = 0; i <= N; ++i) {
            // downward triangle: bottom[i], top[i+1], top[i]
            mesh.insert({ bottom_nodes[i], top_nodes[i + 1], top_nodes[i] });

            // upward triangle: bottom[i], bottom[i+1], top[i+1]
            if (i < N) {
                mesh.insert({ bottom_nodes[i], bottom_nodes[i + 1], top_nodes[i + 1] });
            }
        }
    }

    return top_nodes;
}

// create the initial bottom row of nodes
std::vector<node_t>
create_base_nodes(coord_system_t & coord_system, int N, double l, double x_start, double y)
{
    std::vector<node_t> nodes;
    for (int i = 0; i <= N; ++i) {
        nodes.push_back(mito::geometry::node(coord_system, { x_start + double(i) * l, y }));
    }
    return nodes;
}

int
main(int argc, char * argv[])
{
    // parse command-line argument for num_bottom_segments and num_rows
    int num_bottom_segments = 30;    // default
    int num_rows = 12;               // default
    if (argc >= 2) {
        int val = std::atoi(argv[1]);
        if (val > 0) {
            num_bottom_segments = val;
        } else {
            std::cerr << "Invalid value for num_bottom_segments (must be positive), using default "
                         "num_bottom_segments = 30.\n";
        }
    }
    if (argc >= 3) {
        int val = std::atoi(argv[2]);
        if ((val > 0) && (val % 2 == 0)) {
            num_rows = val;
        } else {
            std::cerr << "Invalid value for num_rows (must be even and positive), using default "
                         "num_rows = 12.\n";
        }
    }

    journal::info_t channel("apps.getting_started");

    // initialize PETSc
    mito::petsc::initialize();

    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    const double L = 10.0;
    const double l = L / num_bottom_segments;
    const double h = std::sqrt(3.0) / 2.0 * l;
    const double y_start = -h * num_rows / 2.0;

    auto mesh = mito::mesh::mesh<triangle_t>();

    // create base row of nodes
    auto current_nodes = create_base_nodes(coord_system, num_bottom_segments, l, 0.0, y_start);

    // stack triangle rows and capture centerline nodes
    double current_y = y_start;
    bool pointing_up = true;
    std::vector<node_t> centerline_nodes;
    const int centerline_row = num_rows / 2;

    for (int row = 0; row < num_rows; ++row) {
        current_y += h;
        current_nodes =
            create_triangle_row(mesh, coord_system, current_nodes, l, h, current_y, pointing_up);

        // capture centerline nodes at the middle row
        if (row == centerline_row - 1) {
            centerline_nodes = current_nodes;
        }

        pointing_up = !pointing_up;    // alternate pattern
    }

    channel << "Created 2D mesh with " << mesh.nCells() << " triangles" << journal::endl;

    // create interface mesh at centerline (1D embedded in 2D)
    auto interface_mesh = create_interface_mesh(centerline_nodes);
    channel << "Interface mesh at centerline: " << interface_mesh.nCells() << " segments"
            << journal::endl;

    // ==================== bulk manifold + function space ====================
    // create a body manifold from the 2D triangle mesh
    auto bulk_manifold = mito::manifolds::manifold(mesh, coord_system);

    // finite element type for bulk triangles
    using bulk_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(bulk_manifold)>;

    // get the boundary mesh
    auto bulk_boundary_mesh = mito::mesh::boundary(mesh);

    // zero field
    auto zero = mito::functions::zero<coordinates_t>;

    // set homogeneous Dirichlet boundary conditions on the bulk boundary
    auto bulk_constraints = mito::constraints::dirichlet_bc(bulk_boundary_mesh, zero);

    // create the bulk function space
    auto bulk_function_space =
        mito::fem::function_space<bulk_element_t>(bulk_manifold, bulk_constraints);

    channel << "Bulk function space: " << bulk_function_space.node_map().size() << " nodes"
            << journal::endl;

    // ==================== interface manifold + function space =================
    // define the normal field for the submanifold
    // for a horizontal centerline, the normal is the vertical direction e_y = (0, 1)
    auto normal = mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, 1.0 });

    // create the submanifold from the interface mesh with the normal field
    auto submanifold = mito::manifolds::submanifold(interface_mesh, coord_system, normal);

    // deduce the finite element type from the submanifold
    using interface_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(submanifold)>;

    // get the boundary mesh
    auto interface_boundary_mesh = mito::mesh::boundary(interface_mesh);

    // set homogeneous Dirichlet boundary conditions at the endpoints
    auto interface_constraints = mito::constraints::dirichlet_bc(interface_boundary_mesh, zero);


    // build the interface function space with a shared node map from the bulk space
    // the discretizer reuses the existing discretization_node_t objects for shared nodes (i.e.,
    // centerline nodes)
    auto function_space = mito::fem::function_space<interface_element_t>(
        submanifold, interface_constraints, bulk_function_space.node_map());

    // ==================== log node maps to verify shared DOFs ==============================
    {
        const auto & bulk_map = bulk_function_space.node_map();
        const auto & interface_map = function_space.node_map();

        channel << "Bulk node_map size: " << bulk_map.size() << journal::endl;
        channel << "Interface node_map size: " << interface_map.size() << journal::endl;

        // count how many interface nodes share the same discretization_node_t with the bulk
        int shared_count = 0;
        int interface_only_count = 0;
        for (const auto & [mesh_node, disc_node] : interface_map) {
            auto it = bulk_map.find(mesh_node);
            if (it != bulk_map.end() && it->second == disc_node) {
                ++shared_count;
            } else {
                ++interface_only_count;
            }
        }
        channel << "Shared discretization nodes (bulk & interface): " << shared_count
                << journal::endl;
        channel << "Interface-only discretization nodes: " << interface_only_count << journal::endl;
    }

    // assemble the weak form for Poisson: -u'' = f
    // LHS: grad-grad block (stiffness matrix)
    auto fem_lhs_block =
        mito::fem::blocks::grad_grad_block<interface_element_t, quadrature_rule_segment_t>();

    // the forcing term f = sin(pi * x)
    constexpr auto x_coord = mito::functions::component<coordinates_t, 0>;
    auto f = mito::functions::sin(std::numbers::pi * x_coord);

    // RHS: source term block
    auto fem_rhs_block =
        mito::fem::blocks::source_term_block<interface_element_t, quadrature_rule_segment_t>(f);

    // create the weak form and add blocks
    auto weakform = mito::fem::weakform<interface_element_t>();
    weakform.add_block(fem_lhs_block);
    weakform.add_block(fem_rhs_block);

    // create the discrete system
    auto discrete_system =
        mito::fem::discrete_system<linear_system_t>("embedded_poisson", function_space, weakform);

    // create a linear solver
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);

    // set solver options
    solver.set_options("-ksp_type preonly -pc_type cholesky");

    // solve the system
    solver.solve();

    channel << "Solved 1D Poisson problem on embedded interface" << journal::endl;

    solver.destroy();

    // get domain boundaries for exact solution
    auto x0 = coord_system.coordinates(centerline_nodes.front()->point());
    auto xL = coord_system.coordinates(centerline_nodes.back()->point());
    const double x_a = x0[0];    // left boundary
    const double x_b = xL[0];    // right boundary

    // the exact solution for -u'' = sin(pi*x) with u(a) = u(b) = 0 is:
    // u(x) = (1/pi²) sin(pi*x) + Cx + D
    const double u_particular_a =
        (1.0 / (std::numbers::pi * std::numbers::pi)) * std::sin(std::numbers::pi * x_a);
    const double u_particular_b =
        (1.0 / (std::numbers::pi * std::numbers::pi)) * std::sin(std::numbers::pi * x_b);
    const double C_exact = (u_particular_a - u_particular_b) / (x_b - x_a);
    const double D_exact = -u_particular_a - C_exact * x_a;

    auto u_exact = (1.0 / (std::numbers::pi * std::numbers::pi))
                     * mito::functions::sin(std::numbers::pi * x_coord)
                 + C_exact * x_coord + D_exact;

#ifdef WITH_VTK
    // get the solution vector
    const auto & solution = discrete_system.solution();

    // build node map for solution retrieval (only interface nodes)
    const auto & node_map = function_space.node_map();

    // build solution lookup by x-coordinate for interpolation onto 2D mesh
    std::vector<std::pair<double, double>> x_u_pairs;
    for (const auto & cnode : centerline_nodes) {
        auto it = node_map.find(cnode);
        if (it != node_map.end()) {
            auto x = coord_system.coordinates(cnode->point());
            x_u_pairs.emplace_back(x[0], solution(it->second));
        }
    }
    std::sort(x_u_pairs.begin(), x_u_pairs.end(), [](const auto & a, const auto & b) {
        return a.first < b.first;
    });

    std::vector<double> x_sorted, u_sorted;
    x_sorted.reserve(x_u_pairs.size());
    u_sorted.reserve(x_u_pairs.size());
    for (const auto & [xv, uv] : x_u_pairs) {
        x_sorted.push_back(xv);
        u_sorted.push_back(uv);
    }

    // linear interpolation helper
    auto interpolate_1d = [](const std::vector<double> & xs, const std::vector<double> & ys,
                             double x) {
        if (x <= xs.front())
            return ys.front();
        if (x >= xs.back())
            return ys.back();

        auto it = std::lower_bound(xs.begin(), xs.end(), x);
        size_t i = std::distance(xs.begin(), it);
        if (i == 0)
            i = 1;

        double t = (x - xs[i - 1]) / (xs[i] - xs[i - 1]);
        return ys[i - 1] + t * (ys[i] - ys[i - 1]);
    };

    // create fields on the 2D triangle mesh
    auto solution_2d =
        mito::discrete::mesh_field<mito::tensor::scalar_t>(mesh, "numerical solution");
    auto exact_2d = mito::discrete::mesh_field<mito::tensor::scalar_t>(mesh, "exact solution");
    auto forcing_2d = mito::discrete::mesh_field<mito::tensor::scalar_t>(mesh, "forcing term");

    // project solution onto 2D mesh nodes based on x-coordinate
    for (auto & [node, value] : solution_2d) {
        auto x = coord_system.coordinates(node->point());
        double x_val = x[0];

        // interpolate numerical solution
        value = interpolate_1d(x_sorted, u_sorted, x_val);

        // exact solution
        exact_2d(node) =
            (1.0 / (std::numbers::pi * std::numbers::pi)) * std::sin(std::numbers::pi * x_val)
            + C_exact * x_val + D_exact;

        // forcing term
        forcing_2d(node) = std::sin(std::numbers::pi * x_val);
    }

    // write VTK file for the full 2D mesh with projected solution
    auto writer = mito::io::vtk::field_writer("getting_started_full", mesh, coord_system);
    writer.record(solution_2d);
    writer.record(exact_2d);
    writer.record(forcing_2d);
    writer.write();

    channel << "VTK file written: getting_started_full.vtu" << journal::endl;

#endif

    // finalize PETSc
    mito::petsc::finalize();

    // all done
    return 0;
}

// end of file
