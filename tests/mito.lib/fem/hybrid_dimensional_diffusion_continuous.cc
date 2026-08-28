// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// hybrid-dimensional diffusion with a continuous interface potential: a conductive layer
// collapsed to y = 0 is embedded in the 2D bulk and shares its degrees of freedom; verified
// against the exact three-layer 1D conduction solution

#include <gtest/gtest.h>
#include <mito.h>


using scalar_t = mito::tensor::scalar_t;
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
using triangle_t = mito::geometry::triangle_t<2>;
using segment_t = mito::geometry::segment_t<2>;
using node_t = mito::geometry::node_t<2>;

// first degree finite elements for the bulk (triangles) and the interface (embedded segments)
using bulk_element_t = mito::fem::finite_element_family<triangle_t, 1>;
using interface_element_t = mito::fem::finite_element_family<segment_t, 1>;

// degree of exactness 2 for the bulk and interface blocks and the L2 error norm
constexpr int doe = 2;

using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;


// the model parameters
struct Parameters {
    scalar_t width = 2.0e-4;
    scalar_t half_thickness = 1.5e-4;    // (L + w) / 2 = (2e-4 + 1e-4) / 2
    scalar_t layer_thickness = 1.0e-4;
    scalar_t kappa_se = 1.0;
    scalar_t phi_bottom = -5.0;
    scalar_t phi_top = 1.0;
    int x_segments = 24;
    int y_segments = 24;
};

// the hybrid-dimensional model removes the layer interior [-h, h] from the mesh
constexpr auto
reduced_half_thickness(const Parameters & parameters) -> scalar_t
{
    return parameters.half_thickness - 0.5 * parameters.layer_thickness;
}

// map the reduced mesh coordinate y to the physical z-coordinate (which has a gap at the layer)
constexpr auto
physical_z_from_reduced_y(const Parameters & parameters, scalar_t reduced_y) -> scalar_t
{
    const auto half_layer_thickness = 0.5 * parameters.layer_thickness;
    if (reduced_y < 0.0) {
        return reduced_y - half_layer_thickness;
    }
    if (reduced_y > 0.0) {
        return reduced_y + half_layer_thickness;
    }
    return 0.0;
}

// the exact potential of the equivalent three-layer 1D conduction problem (normalized by kappa_se)
constexpr auto
analytical_potential(const Parameters & parameters, scalar_t conductivity_ratio, scalar_t z)
    -> scalar_t
{
    const auto h = 0.5 * parameters.layer_thickness;
    const auto denominator = 2.0 * conductivity_ratio * (parameters.half_thickness - h) + 2.0 * h;
    const auto delta_phi = parameters.phi_top - parameters.phi_bottom;

    if (z <= -h) {
        return delta_phi * conductivity_ratio / denominator * (z + parameters.half_thickness)
             + parameters.phi_bottom;
    }
    if (z >= h) {
        return delta_phi * conductivity_ratio / denominator * (z - parameters.half_thickness)
             + parameters.phi_top;
    }
    return delta_phi / denominator * z + 0.5 * (parameters.phi_bottom + parameters.phi_top);
}

// the result of one conductivity-ratio case
struct CaseResult {
    scalar_t l2_error;
    scalar_t max_profile_error;
    scalar_t max_row_spread;
};

auto
run_case(const Parameters & parameters, scalar_t conductivity_ratio) -> CaseResult
{
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();
    auto mesh = mito::mesh::mesh<triangle_t>();

    // build the structured mesh of the reduced domain (layer collapsed to y = 0)
    const auto domain_half_height = reduced_half_thickness(parameters);
    const auto dx = parameters.width / static_cast<scalar_t>(parameters.x_segments);
    const auto dy = 2.0 * domain_half_height / static_cast<scalar_t>(parameters.y_segments);

    auto node_grid = std::vector<std::vector<node_t>>{};
    node_grid.reserve(parameters.y_segments + 1);
    for (int j = 0; j <= parameters.y_segments; ++j) {
        const auto y = -domain_half_height + static_cast<scalar_t>(j) * dy;
        auto row = std::vector<node_t>{};
        row.reserve(parameters.x_segments + 1);
        for (int i = 0; i <= parameters.x_segments; ++i) {
            const auto x = static_cast<scalar_t>(i) * dx;
            row.push_back(mito::geometry::node(coord_system, { x, y }));
        }
        node_grid.push_back(std::move(row));
    }

    // two triangles per structured cell, with alternating diagonals
    for (int j = 0; j < parameters.y_segments; ++j) {
        for (int i = 0; i < parameters.x_segments; ++i) {
            const auto & node_00 = node_grid[j][i];
            const auto & node_10 = node_grid[j][i + 1];
            const auto & node_01 = node_grid[j + 1][i];
            const auto & node_11 = node_grid[j + 1][i + 1];
            if ((i + j) % 2 == 0) {
                mesh.insert({ node_00, node_10, node_11 });
                mesh.insert({ node_00, node_11, node_01 });
            } else {
                mesh.insert({ node_00, node_10, node_01 });
                mesh.insert({ node_10, node_11, node_01 });
            }
        }
    }

    // the Dirichlet boundary consists of the bottom and top rows (natural conditions on the sides)
    auto boundary_mesh = mito::mesh::mesh<segment_t>();
    for (int i = 0; i < parameters.x_segments; ++i) {
        boundary_mesh.insert({ node_grid.front()[i], node_grid.front()[i + 1] });
        boundary_mesh.insert({ node_grid.back()[i], node_grid.back()[i + 1] });
    }

    // the interface mesh on the layer line y = 0
    assert(parameters.y_segments % 2 == 0);
    const auto interface_row = parameters.y_segments / 2;
    auto interface_mesh = mito::mesh::mesh<segment_t>();
    for (int i = 0; i < parameters.x_segments; ++i) {
        interface_mesh.insert({ node_grid[interface_row][i], node_grid[interface_row][i + 1] });
    }

    // the bulk manifold
    auto bulk_manifold = mito::manifolds::manifold(mesh, coord_system, metric_space_t::w);

    // the interface manifold (a 1D submanifold embedded in 2D with normal along y)
    auto normal = mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, 1.0 });
    auto interface_manifold = mito::manifolds::submanifold(interface_mesh, coord_system, normal);

    // the prescribed potentials on the bottom and top boundaries
    auto dirichlet_values =
        mito::functions::function([parameters](const coordinates_t & coordinates) -> scalar_t {
            return coordinates[1] < 0.0 ? parameters.phi_bottom : parameters.phi_top;
        });
    auto bulk_constraints = mito::constraints::dirichlet_bc(boundary_mesh, dirichlet_values);

    // no constraints on the interface (an empty set of constrained nodes)
    auto empty_node_set = std::set<node_t>{};
    auto interface_constraints = mito::constraints::dirichlet_bc(empty_node_set, dirichlet_values);

    // the bulk function space
    auto bulk_space = mito::fem::function_space<bulk_element_t>(bulk_manifold, bulk_constraints);

    // the interface function space, sharing the bulk discretization nodes on the crack line
    auto interface_space = mito::fem::function_space<interface_element_t>(
        interface_manifold, interface_constraints, bulk_space.node_map());

    // the zero forcing field
    auto zero = mito::functions::zero<coordinates_t>;

    // TOFIX: the physical conductivities are applied through {Contribution.coefficient};
    // fold them into the blocks themselves with {BlockProduct} instead
    auto identity = mito::functions::identity<coordinates_t, 2>();

    // the bulk weakform: diffusion with no source
    auto bulk_lhs = mito::fem::blocks::diffusion<bulk_element_t, doe>(identity);
    auto bulk_rhs = mito::fem::blocks::source<bulk_element_t, doe>(zero);
    auto bulk_weakform = mito::fem::weakform(bulk_lhs, bulk_rhs);

    // the interface weakform: tangential diffusion along the crack with no source
    auto interface_lhs = mito::fem::blocks::diffusion<interface_element_t, doe>(identity);
    auto interface_rhs = mito::fem::blocks::source<interface_element_t, doe>(zero);
    auto interface_weakform = mito::fem::weakform(interface_lhs, interface_rhs);

    // the discrete system: bulk diffusion scaled by kappa_se, tangential conductance of the
    // collapsed layer scaled by -w * kappa_m
    const auto kappa_m = conductivity_ratio * parameters.kappa_se;
    auto discrete_system = mito::fem::discrete_system<linear_system_t>(
        "hybrid_dimensional_diffusion",
        mito::fem::Contribution{ bulk_space, bulk_weakform, parameters.kappa_se },
        mito::fem::Contribution{ interface_space, interface_weakform,
                                 -parameters.layer_thickness * kappa_m });

    // solve
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);
    solver.set_options("-ksp_type preonly -pc_type lu");
    solver.solve();
    solver.destroy();

    const auto & solution = discrete_system.solution();

    // the analytical potential projected on the reduced domain
    auto exact_projected = mito::functions::function(
        [parameters, conductivity_ratio](const coordinates_t & coordinates) -> scalar_t {
            return analytical_potential(
                parameters, conductivity_ratio,
                physical_z_from_reduced_y(parameters, coordinates[1]));
        });

    // the L2 norm of the error over the bulk
    const auto l2_error = mito::fem::compute_l2_norm<doe>(bulk_space, solution, exact_projected);

    // sample the numerical solution on the mid column and check x-uniformity on each row
    const auto mid_column = parameters.x_segments / 2;
    scalar_t max_profile_error = 0.0;
    scalar_t max_row_spread = 0.0;

    for (int j = 0; j <= parameters.y_segments; ++j) {
        scalar_t row_min = std::numeric_limits<scalar_t>::max();
        scalar_t row_max = std::numeric_limits<scalar_t>::lowest();
        for (int i = 0; i <= parameters.x_segments; ++i) {
            const auto value = solution(bulk_space.node_map().at(node_grid[j][i]));
            row_min = std::min(row_min, value);
            row_max = std::max(row_max, value);
        }
        max_row_spread = std::max(max_row_spread, row_max - row_min);

        // the interface row has no counterpart in the exact layered solution
        if (j == interface_row) {
            continue;
        }

        const auto & node = node_grid[j][mid_column];
        const auto coordinates = coord_system.coordinates(node->point());
        const auto numerical = solution(bulk_space.node_map().at(node));
        const auto exact = analytical_potential(
            parameters, conductivity_ratio, physical_z_from_reduced_y(parameters, coordinates[1]));
        max_profile_error = std::max(max_profile_error, std::abs(numerical - exact));
    }

    return CaseResult{ l2_error, max_profile_error, max_row_spread };
}


TEST(Fem, HybridDimensionalDiffusionContinuous)
{
    // initialize PETSc
    mito::petsc::initialize();

    const auto parameters = Parameters{};

    // expected errors: the continuous formulation is accurate only for highly conductive layers
    struct Expected {
        scalar_t ratio;
        scalar_t l2_error;
        scalar_t max_profile_error;
    };
    constexpr auto expected = std::array<Expected, 5>{
        Expected{ 0.001, 3.46e-04, 2.74 },     Expected{ 0.5, 1.73e-04, 1.38 },
        Expected{ 1.0, 1.15e-04, 0.92 },       Expected{ 2.0, 6.93e-05, 0.55 },
        Expected{ 1000.0, 1.73e-07, 1.4e-03 },
    };

    for (const auto & expectation : expected) {
        const auto result = run_case(parameters, expectation.ratio);

        // the solution is uniform in x on every row (the problem is one-dimensional in y)
        EXPECT_LT(result.max_row_spread, 1.0e-9);

        // the errors match the reference values within 2%
        EXPECT_NEAR(result.l2_error, expectation.l2_error, 0.02 * expectation.l2_error);
        EXPECT_NEAR(
            result.max_profile_error, expectation.max_profile_error,
            0.02 * expectation.max_profile_error);
    }

    // finalize PETSc
    mito::petsc::finalize();
}


// end of file
