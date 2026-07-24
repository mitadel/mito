// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include <mito.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace cg_chapter5_verification {

    using scalar_t = mito::tensor::scalar_t;
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
    using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
    using triangle_t = mito::geometry::triangle_t<2>;
    using segment_t = mito::geometry::segment_t<2>;
    using node_t = mito::geometry::node_t<2>;

    constexpr int degree = 1;
    constexpr int bulk_doe = 2;
    constexpr int interface_doe = 2;

    using bulk_reference_simplex_t = mito::geometry::reference_triangle_t;
    using interface_reference_simplex_t = mito::geometry::reference_segment_t;
    using bulk_quadrature_rule_t = mito::quadrature::quadrature_rule_t<
        mito::quadrature::GAUSS, bulk_reference_simplex_t, bulk_doe>;
    using interface_quadrature_rule_t = mito::quadrature::quadrature_rule_t<
        mito::quadrature::GAUSS, interface_reference_simplex_t, interface_doe>;
    using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
    using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

    struct Parameters {
        scalar_t width = 2.0e-4;

        scalar_t half_thickness = 1.5e-4;    // (L + w) / 2 = (2e-4 + 1e-4) / 2

        scalar_t crack_opening = 1.0e-4;
        scalar_t kappa_se = 1.0;
        scalar_t phi_bottom = -5.0;
        scalar_t phi_top = 1.0;
        int x_segments = 24;
        int y_segments = 24;
        std::string solver_options = "-ksp_type preonly -pc_type lu";
    };

    struct OutputOptions {
        bool write_profile_csv = false;
        bool write_vtk = false;
        std::string label_prefix = "cg_chapter5_verification";
    };

    struct ProfileSample {
        scalar_t reduced_y;     // mesh coordinate
        scalar_t physical_z;    // physical coordinate (has gap at crack interior)
        scalar_t numerical;
        scalar_t exact;
        scalar_t absolute_error;
    };
    // NOTE: To match Timothy's Figure 5.5/5.6, plot numerical vs. physical_z,
    // not vs. reduced_y

    struct CaseResult {
        scalar_t conductivity_ratio;
        scalar_t l2_error;
        scalar_t max_profile_error;
        scalar_t max_row_spread;
        std::vector<ProfileSample> profile;
    };

    constexpr auto primary_ratios() -> std::array<scalar_t, 3>
    {
        return { 1.0, 2.0, 1000.0 };
    }

    constexpr auto diagnostic_ratios() -> std::array<scalar_t, 2>
    {
        return { 0.5, 0.001 };
    }

    constexpr auto reduced_half_thickness(const Parameters & parameters) -> scalar_t
    {
        // the hybrid-dimensional model removes the crack interior [-h, h] from the mesh
        return parameters.half_thickness - 0.5 * parameters.crack_opening;
    }

    inline auto ratio_tag(const scalar_t ratio) -> std::string
    {
        std::ostringstream stream;
        stream << std::setprecision(12) << ratio;

        auto tag = stream.str();
        std::replace(tag.begin(), tag.end(), '.', 'p');
        std::replace(tag.begin(), tag.end(), '-', 'm');
        return tag;
    }

    inline auto create_polyline_mesh(const std::vector<node_t> & nodes)
        -> mito::mesh::mesh_t<segment_t>
    {
        auto mesh = mito::mesh::mesh<segment_t>();
        for (std::size_t index = 0; index + 1 < nodes.size(); ++index) {
            mesh.insert({ nodes[index], nodes[index + 1] });
        }
        return mesh;
    }

    inline auto physical_z_from_reduced_y(const Parameters & parameters, const scalar_t reduced_y)
        -> scalar_t
    {
        // map reduced mesh coordinate y to physical z-coordinate for eq. 5.30 evaluation
        const auto half_crack_opening = 0.5 * parameters.crack_opening;

        if (reduced_y < 0.0) {
            return reduced_y - half_crack_opening;
        }

        if (reduced_y > 0.0) {
            return reduced_y + half_crack_opening;
        }

        return 0.0;
    }

    inline auto analytical_potential(
        const Parameters & parameters, const scalar_t conductivity_ratio, const scalar_t z)
        -> scalar_t
    {
        const auto h = 0.5 * parameters.crack_opening;
        // normalized form of eq. 5.30 denominator (divided through by kappa_se):
        // 2 * (k_m/k_SE) * (L - h) + 2 * h
        const auto denominator =
            2.0 * conductivity_ratio * (parameters.half_thickness - h) + 2.0 * h;
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

    inline auto build_numerical_field(const auto & function_space, const auto & discrete_solution)
    {
        // create a fem_field (required for localization in compute_l2_norm)
        auto numerical_field = function_space.template fem_field<scalar_t>("numerical_solution");

        // copy all values from discrete_solution (which already has constrained nodes populated)
        for (const auto & [mesh_node, discretization_node] : function_space.node_map()) {
            numerical_field(discretization_node) = discrete_solution(discretization_node);
        }

        return numerical_field;
    }

    inline auto write_profile_csv(
        const std::string & prefix, const scalar_t conductivity_ratio,
        const std::vector<ProfileSample> & profile) -> void
    {
        std::ofstream stream(prefix + "_ratio_" + ratio_tag(conductivity_ratio) + ".csv");
        stream << "reduced_y,physical_z,numerical,exact,absolute_error\n";
        stream << std::setprecision(16);

        for (const auto & sample : profile) {
            stream << sample.reduced_y << ',' << sample.physical_z << ',' << sample.numerical << ','
                   << sample.exact << ',' << sample.absolute_error << '\n';
        }
    }

    inline auto run_case(
        const Parameters & parameters, const scalar_t conductivity_ratio,
        const OutputOptions & output_options = {}) -> CaseResult
    {
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();
        auto mesh = mito::mesh::mesh<triangle_t>();

        // build the reduced mesh (crack interior removed, interface collapsed to y=0)
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

        const auto interface_row = parameters.y_segments / 2;
        const auto & bottom_nodes = node_grid.front();
        const auto & top_nodes = node_grid.back();
        const auto & interface_nodes = node_grid[interface_row];

        auto bulk_boundary_mesh = create_polyline_mesh(bottom_nodes);
        auto top_boundary_mesh = create_polyline_mesh(top_nodes);
        for (const auto & segment : top_boundary_mesh.cells()) {
            bulk_boundary_mesh.insert(segment.nodes());
        }

        auto interface_mesh = create_polyline_mesh(interface_nodes);
        auto empty_interface_boundary = mito::mesh::mesh<segment_t>();

        auto bulk_manifold = mito::manifolds::manifold(mesh, coord_system);
        auto normal =
            mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, 1.0 });
        auto interface_manifold =
            mito::manifolds::submanifold(interface_mesh, coord_system, normal);

        using bulk_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(bulk_manifold)>;
        using interface_element_t =
            mito::fem::isoparametric_simplex_t<degree, decltype(interface_manifold)>;

        auto dirichlet_values =
            mito::functions::function([parameters](const coordinates_t & coordinates) -> scalar_t {
                return coordinates[1] < 0.0 ? parameters.phi_bottom : parameters.phi_top;
            });

        auto bulk_constraints =
            mito::constraints::dirichlet_bc(bulk_boundary_mesh, dirichlet_values);
        auto interface_constraints =
            mito::constraints::dirichlet_bc(empty_interface_boundary, dirichlet_values);

        auto bulk_space =
            mito::fem::function_space<bulk_element_t>(bulk_manifold, bulk_constraints);
        auto interface_space = mito::fem::function_space<interface_element_t>(
            interface_manifold, interface_constraints, bulk_space.node_map());

        auto bulk_lhs =
            mito::fem::blocks::grad_grad_block<bulk_element_t, bulk_quadrature_rule_t>();
        auto interface_lhs =
            mito::fem::blocks::grad_grad_block<interface_element_t, interface_quadrature_rule_t>();

        auto bulk_weakform = mito::fem::weakform<bulk_element_t>();
        bulk_weakform.add_block(bulk_lhs);

        auto interface_weakform = mito::fem::weakform<interface_element_t>();
        interface_weakform.add_block(interface_lhs);

        const auto kappa_m = conductivity_ratio * parameters.kappa_se;
        auto discrete_system = mito::fem::discrete_system<linear_system_t>(
            output_options.label_prefix + ".ratio_" + ratio_tag(conductivity_ratio),
            mito::fem::Contribution{ bulk_space, bulk_weakform, parameters.kappa_se },
            mito::fem::Contribution{ interface_space, interface_weakform,
                                     -parameters.crack_opening * kappa_m });

        // populate prescribed Dirichlet values for constrained nodes
        // loop over constrained nodes in bulk space
        for (const auto & disc_node : bulk_space.constrained_nodes()) {
            // find the corresponding mesh node
            for (const auto & [mesh_node, discretization_node] : bulk_space.node_map()) {
                if (discretization_node == disc_node) {
                    const auto coords = coord_system.coordinates(mesh_node->point());
                    const auto prescribed_value = dirichlet_values(coords);
                    discrete_system.set_prescribed_value(disc_node, prescribed_value);
                    break;
                }
            }
        }
        // loop over constrained nodes in interface space (if any)
        for (const auto & disc_node : interface_space.constrained_nodes()) {
            // find the corresponding mesh node
            for (const auto & [mesh_node, discretization_node] : interface_space.node_map()) {
                if (discretization_node == disc_node) {
                    const auto coords = coord_system.coordinates(mesh_node->point());
                    const auto prescribed_value = dirichlet_values(coords);
                    discrete_system.set_prescribed_value(disc_node, prescribed_value);
                    break;
                }
            }
        }

        auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);
        solver.set_options(parameters.solver_options);
        solver.solve();
        solver.destroy();

        // convert discrete_solution to fem_field
        // discrete_system.solution() already has constrained nodes populated
        const auto & discrete_solution = discrete_system.solution();
        auto numerical_field = build_numerical_field(bulk_space, discrete_solution);

        auto exact_projected = mito::functions::function(
            [parameters, conductivity_ratio](const coordinates_t & coordinates) -> scalar_t {
                return analytical_potential(
                    parameters, conductivity_ratio,
                    physical_z_from_reduced_y(parameters, coordinates[1]));
            });

        const auto l2_error = mito::fem::compute_l2_norm<bulk_quadrature_rule_t>(
            bulk_space, numerical_field, exact_projected);

        std::vector<ProfileSample> profile;
        profile.reserve(parameters.y_segments);    // y_segments, not +1 (skipping interface row)

        const auto mid_column = parameters.x_segments / 2;
        scalar_t max_profile_error = 0.0;
        scalar_t max_row_spread = 0.0;

        for (int j = 0; j <= parameters.y_segments; ++j) {
            scalar_t row_min = std::numeric_limits<scalar_t>::max();
            scalar_t row_max = std::numeric_limits<scalar_t>::lowest();

            for (int i = 0; i <= parameters.x_segments; ++i) {
                const auto & node = node_grid[j][i];
                const auto value = numerical_field(bulk_space.node_map().at(node));
                row_min = std::min(row_min, value);
                row_max = std::max(row_max, value);
            }

            max_row_spread = std::max(max_row_spread, row_max - row_min);

            // skip interface row
            if (j == interface_row) {
                continue;
            }

            const auto & node = node_grid[j][mid_column];
            const auto coordinates = coord_system.coordinates(node->point());
            const auto numerical = numerical_field(bulk_space.node_map().at(node));
            const auto physical_z = physical_z_from_reduced_y(parameters, coordinates[1]);
            const auto exact = analytical_potential(parameters, conductivity_ratio, physical_z);
            const auto absolute_error = std::abs(numerical - exact);

            max_profile_error = std::max(max_profile_error, absolute_error);
            profile.push_back({ coordinates[1], physical_z, numerical, exact, absolute_error });
        }

        // sort profile by physical_z to ensure monotonic x-axis for plotting
        std::sort(
            profile.begin(), profile.end(), [](const ProfileSample & a, const ProfileSample & b) {
                return a.physical_z < b.physical_z;
            });

        if (output_options.write_profile_csv) {
            write_profile_csv(output_options.label_prefix, conductivity_ratio, profile);
        }

#ifdef WITH_VTK
        if (output_options.write_vtk) {
            auto numerical_mesh_field =
                mito::discrete::mesh_field<scalar_t>(mesh, "numerical_solution");
            auto exact_mesh_field =
                mito::discrete::mesh_field<scalar_t>(mesh, "analytical_reference");
            auto error_mesh_field = mito::discrete::mesh_field<scalar_t>(mesh, "absolute_error");
            auto physical_z_field = mito::discrete::mesh_field<scalar_t>(mesh, "physical_z");

            for (auto & [mesh_node, value] : numerical_mesh_field) {
                const auto coordinates = coord_system.coordinates(mesh_node->point());
                const auto reduced_y = coordinates[1];
                const auto physical_z = physical_z_from_reduced_y(parameters, reduced_y);
                const auto numerical = numerical_field(bulk_space.node_map().at(mesh_node));
                const auto exact = analytical_potential(parameters, conductivity_ratio, physical_z);

                value = numerical;
                exact_mesh_field(mesh_node) = exact;
                error_mesh_field(mesh_node) = std::abs(numerical - exact);

                // skip interface nodes
                if (std::abs(reduced_y) > 1.0e-12) {
                    physical_z_field(mesh_node) = physical_z;
                }
            }

            auto writer = mito::io::vtk::field_writer(
                output_options.label_prefix + "_ratio_" + ratio_tag(conductivity_ratio), mesh,
                coord_system);
            writer.record(numerical_mesh_field);
            writer.record(exact_mesh_field);
            writer.record(error_mesh_field);
            writer.record(physical_z_field);
            writer.write();
        }
#endif

        return CaseResult{ conductivity_ratio, l2_error, max_profile_error, max_row_spread,
                           profile };
    }

}    // namespace cg_chapter5_verification
