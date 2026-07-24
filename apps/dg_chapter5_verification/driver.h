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
#include <map>
#include <sstream>
#include <string>
#include <vector>

// hybrid-dimensional interface coupling (Timothy's thesis sec. 4.4–4.5)
// not a classical single-domain DG, bulk subdomains are continuous
namespace hybrid_interface_chapter5_verification {

    using scalar_t = mito::tensor::scalar_t;
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
    using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
    using triangle_t = mito::geometry::triangle_t<2>;
    using segment_t = mito::geometry::segment_t<2>;
    using node_t = mito::geometry::node_t<2>;

    constexpr int degree = 1;
    constexpr int bulk_doe = 2;
    constexpr int n_bulk_nodes = 3;

    using bulk_reference_simplex_t = mito::geometry::reference_triangle_t;
    using bulk_quadrature_rule_t = mito::quadrature::quadrature_rule_t<
        mito::quadrature::GAUSS, bulk_reference_simplex_t, bulk_doe>;
    using linear_system_t = mito::matrix_solvers::petsc::linear_system_t;
    using matrix_solver_t = mito::matrix_solvers::petsc::ksp_t;

    struct Parameters {
        scalar_t width = 2.0e-4;
        scalar_t half_thickness = 1.5e-4;
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
        std::string label_prefix = "dg_chapter5_verification";
    };

    struct ProfileSample {
        scalar_t reduced_y;
        scalar_t physical_z;
        scalar_t numerical;
        scalar_t exact;
        scalar_t absolute_error;
    };

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

    inline auto analytical_potential(
        const Parameters & parameters, const scalar_t conductivity_ratio, const scalar_t z)
        -> scalar_t
    {
        const auto h = 0.5 * parameters.crack_opening;
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
        auto channel = journal::info_t("dg_chapter5_verification");
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        const auto domain_half_height = reduced_half_thickness(parameters);
        const auto dx = parameters.width / static_cast<scalar_t>(parameters.x_segments);
        const auto dy = 2.0 * domain_half_height / static_cast<scalar_t>(parameters.y_segments);
        const auto half_crack = 0.5 * parameters.crack_opening;
        const auto interface_row = parameters.y_segments / 2;

        // build the node grid
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

        // build top and bottom half-meshes directly from the grid
        // both include y=0, each side has its own DOFs at the interface
        auto top_mesh = mito::mesh::mesh<triangle_t>();
        for (int j = interface_row; j < parameters.y_segments; ++j) {
            for (int i = 0; i < parameters.x_segments; ++i) {
                const auto & n00 = node_grid[j][i];
                const auto & n10 = node_grid[j][i + 1];
                const auto & n01 = node_grid[j + 1][i];
                const auto & n11 = node_grid[j + 1][i + 1];

                if ((i + j) % 2 == 0) {
                    top_mesh.insert({ n00, n10, n11 });
                    top_mesh.insert({ n00, n11, n01 });
                } else {
                    top_mesh.insert({ n00, n10, n01 });
                    top_mesh.insert({ n10, n11, n01 });
                }
            }
        }

        auto bottom_mesh = mito::mesh::mesh<triangle_t>();
        for (int j = 0; j < interface_row; ++j) {
            for (int i = 0; i < parameters.x_segments; ++i) {
                const auto & n00 = node_grid[j][i];
                const auto & n10 = node_grid[j][i + 1];
                const auto & n01 = node_grid[j + 1][i];
                const auto & n11 = node_grid[j + 1][i + 1];

                if ((i + j) % 2 == 0) {
                    bottom_mesh.insert({ n00, n10, n11 });
                    bottom_mesh.insert({ n00, n11, n01 });
                } else {
                    bottom_mesh.insert({ n00, n10, n01 });
                    bottom_mesh.insert({ n10, n11, n01 });
                }
            }
        }

        // boundary meshes for Dirichlet BCs (outer boundaries only, not the interface)
        auto top_boundary = create_polyline_mesh(node_grid.back());
        auto bottom_boundary = create_polyline_mesh(node_grid.front());

        // manifolds
        auto top_manifold = mito::manifolds::manifold(top_mesh, coord_system);
        auto bottom_manifold = mito::manifolds::manifold(bottom_mesh, coord_system);

        using bulk_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(top_manifold)>;

        // Dirichlet BC function
        auto dirichlet_values =
            mito::functions::function([parameters](const coordinates_t & coords) -> scalar_t {
                return coords[1] < 0.0 ? parameters.phi_bottom : parameters.phi_top;
            });

        auto top_bc = mito::constraints::dirichlet_bc(top_boundary, dirichlet_values);
        auto bottom_bc = mito::constraints::dirichlet_bc(bottom_boundary, dirichlet_values);

        // two independent function spaces
        auto top_space = mito::fem::function_space<bulk_element_t>(top_manifold, top_bc);
        auto bottom_space = mito::fem::function_space<bulk_element_t>(bottom_manifold, bottom_bc);

        // bulk weakforms
        auto bulk_lhs =
            mito::fem::blocks::grad_grad_block<bulk_element_t, bulk_quadrature_rule_t>();
        auto weakform_top = mito::fem::weakform<bulk_element_t>();
        weakform_top.add_block(bulk_lhs);
        auto weakform_bottom = mito::fem::weakform<bulk_element_t>();
        weakform_bottom.add_block(bulk_lhs);

        // coupled discrete system
        auto discrete_system = mito::fem::discrete_system<linear_system_t>(
            "hybrid_interface_ratio_" + ratio_tag(conductivity_ratio),
            mito::fem::Contribution{ top_space, weakform_top, parameters.kappa_se },
            mito::fem::Contribution{ bottom_space, weakform_bottom, parameters.kappa_se });

        channel << "Top + bottom DOFs: " << discrete_system.n_equations() << journal::endl;

        // Dirichlet for outer boundaries (not the interface)
        auto prescribe_space = [&](const auto & space, const auto & dirichlet_fn) {
            for (const auto & constrained_node : space.constrained_nodes()) {
                for (const auto & [mesh_node, disc_node] : space.node_map()) {
                    if (disc_node == constrained_node) {
                        discrete_system.set_prescribed_value(
                            constrained_node,
                            dirichlet_fn(coord_system.coordinates(mesh_node->point())));
                        break;
                    }
                }
            }
        };
        prescribe_space(top_space, dirichlet_values);
        prescribe_space(bottom_space, dirichlet_values);

        discrete_system.assemble();

        // interface coupling (eqs. 4.122 and 4.132): jump-jump + average-average on 1D segments
        const auto kappa_m = conductivity_ratio * parameters.kappa_se;
        const auto crack_coeff = kappa_m / parameters.crack_opening;
        const auto diff_coeff = parameters.crack_opening * kappa_m / 4.0;

        auto iface_sample = mito::fem::make_embedded_interface_segment_element(
            coord_system, top_space, node_grid[interface_row][0], node_grid[interface_row][1]);
        using interface_element_t = decltype(iface_sample);

        mito::fem::blocks::InterfaceJumpJumpMassBlock<bulk_element_t, interface_element_t> jj_block(
            crack_coeff);
        mito::fem::blocks::InterfaceAverageAverageGradBlock<bulk_element_t, interface_element_t>
            aa_block(diff_coeff);
        mito::fem::InterfaceWeakform<bulk_element_t, interface_element_t, 4> iface_wf;
        iface_wf.add_block(jj_block);
        iface_wf.add_block(aa_block);

        auto interface_row_nodes = std::vector<node_t>{};
        interface_row_nodes.reserve(static_cast<std::size_t>(parameters.x_segments + 1));
        for (int i = 0; i <= parameters.x_segments; ++i) {
            interface_row_nodes.push_back(node_grid[interface_row][i]);
        }
        auto pairings = mito::fem::build_interface_pairings(
            top_space, bottom_space, interface_row_nodes, parameters.x_segments);

        mito::fem::assemble_interface(
            discrete_system, coord_system, top_space, bottom_space, iface_wf, 1.0, pairings);

        auto ksp = matrix_solver_t(discrete_system.linear_system());
        ksp.create();
        ksp.set_options(parameters.solver_options);
        ksp.solve();

        discrete_system.read_solution();
        ksp.destroy();
        const auto & global_solution = discrete_system.solution();

        auto top_field = top_space.template fem_field<scalar_t>("top_solution");
        auto bottom_field = bottom_space.template fem_field<scalar_t>("bottom_solution");

        for (const auto & [mesh_node, disc_node] : top_space.node_map()) {
            top_field(disc_node) = global_solution(disc_node);
        }
        for (const auto & [mesh_node, disc_node] : bottom_space.node_map()) {
            bottom_field(disc_node) = global_solution(disc_node);
        }

        // compute L2 error on each half separately, then combine
        // y=0 on the top side maps to z=+h, y=0 on the bottom side maps to z=-h
        auto exact_top = mito::functions::function(
            [parameters, conductivity_ratio, half_crack](const coordinates_t & coords) -> scalar_t {
                const auto z = coords[1] + half_crack;
                return analytical_potential(parameters, conductivity_ratio, z);
            });

        auto exact_bottom = mito::functions::function(
            [parameters, conductivity_ratio, half_crack](const coordinates_t & coords) -> scalar_t {
                const auto z = coords[1] - half_crack;
                return analytical_potential(parameters, conductivity_ratio, z);
            });

        const auto l2_top =
            mito::fem::compute_l2_norm<bulk_quadrature_rule_t>(top_space, top_field, exact_top);
        const auto l2_bottom = mito::fem::compute_l2_norm<bulk_quadrature_rule_t>(
            bottom_space, bottom_field, exact_bottom);
        const auto l2_error = std::sqrt(l2_top * l2_top + l2_bottom * l2_bottom);

        // profile extraction: line at x = width/2
        std::vector<ProfileSample> profile;
        profile.reserve(parameters.y_segments + 2);

        const auto mid_column = parameters.x_segments / 2;
        scalar_t max_profile_error = 0.0;
        scalar_t max_row_spread = 0.0;

        // bottom half profile (rows 0 to interface_row)
        for (int j = 0; j <= interface_row; ++j) {
            const auto & mid_node = node_grid[j][mid_column];
            const auto coords = coord_system.coordinates(mid_node->point());
            const auto y = coords[1];
            const auto z = y - half_crack;
            const auto numerical = bottom_field(bottom_space.node_map().at(mid_node));
            const auto exact = analytical_potential(parameters, conductivity_ratio, z);
            const auto err = std::abs(numerical - exact);
            max_profile_error = std::max(max_profile_error, err);
            profile.push_back({ y, z, numerical, exact, err });

            scalar_t row_min = std::numeric_limits<scalar_t>::max();
            scalar_t row_max = std::numeric_limits<scalar_t>::lowest();
            for (int i = 0; i <= parameters.x_segments; ++i) {
                const auto val = bottom_field(bottom_space.node_map().at(node_grid[j][i]));
                row_min = std::min(row_min, val);
                row_max = std::max(row_max, val);
            }
            max_row_spread = std::max(max_row_spread, row_max - row_min);
        }

        // top half profile (rows interface_row to y_segments)
        for (int j = interface_row; j <= parameters.y_segments; ++j) {
            const auto & mid_node = node_grid[j][mid_column];
            const auto coords = coord_system.coordinates(mid_node->point());
            const auto y = coords[1];
            const auto z = y + half_crack;
            const auto numerical = top_field(top_space.node_map().at(mid_node));
            const auto exact = analytical_potential(parameters, conductivity_ratio, z);
            const auto err = std::abs(numerical - exact);
            max_profile_error = std::max(max_profile_error, err);
            profile.push_back({ y, z, numerical, exact, err });

            scalar_t row_min = std::numeric_limits<scalar_t>::max();
            scalar_t row_max = std::numeric_limits<scalar_t>::lowest();
            for (int i = 0; i <= parameters.x_segments; ++i) {
                const auto val = top_field(top_space.node_map().at(node_grid[j][i]));
                row_min = std::min(row_min, val);
                row_max = std::max(row_max, val);
            }
            max_row_spread = std::max(max_row_spread, row_max - row_min);
        }

        // sort profile by physical_z for plotting
        std::sort(
            profile.begin(), profile.end(), [](const ProfileSample & a, const ProfileSample & b) {
                return a.physical_z < b.physical_z;
            });

        if (output_options.write_profile_csv) {
            write_profile_csv(output_options.label_prefix, conductivity_ratio, profile);
        }

#ifdef WITH_VTK
        if (output_options.write_vtk) {
            const auto tag = ratio_tag(conductivity_ratio);

            // single VTK mesh with duplicated interface nodes so the
            // potential discontinuity at the crack is preserved in one file

            auto dup_interface = std::vector<node_t>{};
            dup_interface.reserve(parameters.x_segments + 1);
            for (int i = 0; i <= parameters.x_segments; ++i) {
                const auto c = coord_system.coordinates(node_grid[interface_row][i]->point());
                dup_interface.push_back(mito::geometry::node(coord_system, { c[0], c[1] }));
            }

            auto vtk_mesh = mito::mesh::mesh<triangle_t>();

            // top half: original nodes
            for (int j = interface_row; j < parameters.y_segments; ++j) {
                for (int i = 0; i < parameters.x_segments; ++i) {
                    const auto & n00 = node_grid[j][i];
                    const auto & n10 = node_grid[j][i + 1];
                    const auto & n01 = node_grid[j + 1][i];
                    const auto & n11 = node_grid[j + 1][i + 1];
                    if ((i + j) % 2 == 0) {
                        vtk_mesh.insert({ n00, n10, n11 });
                        vtk_mesh.insert({ n00, n11, n01 });
                    } else {
                        vtk_mesh.insert({ n00, n10, n01 });
                        vtk_mesh.insert({ n10, n11, n01 });
                    }
                }
            }

            // bottom half: duplicate nodes replace the interface row
            for (int j = 0; j < interface_row; ++j) {
                for (int i = 0; i < parameters.x_segments; ++i) {
                    const auto & n00 = node_grid[j][i];
                    const auto & n10 = node_grid[j][i + 1];
                    const auto & n01 =
                        (j + 1 == interface_row) ? dup_interface[i] : node_grid[j + 1][i];
                    const auto & n11 =
                        (j + 1 == interface_row) ? dup_interface[i + 1] : node_grid[j + 1][i + 1];
                    if ((i + j) % 2 == 0) {
                        vtk_mesh.insert({ n00, n10, n11 });
                        vtk_mesh.insert({ n00, n11, n01 });
                    } else {
                        vtk_mesh.insert({ n00, n10, n01 });
                        vtk_mesh.insert({ n10, n11, n01 });
                    }
                }
            }

            auto num_mf = mito::discrete::mesh_field<scalar_t>(vtk_mesh, "numerical_solution");
            auto exact_mf = mito::discrete::mesh_field<scalar_t>(vtk_mesh, "analytical_reference");
            auto err_mf = mito::discrete::mesh_field<scalar_t>(vtk_mesh, "absolute_error");
            auto pz_mf = mito::discrete::mesh_field<scalar_t>(vtk_mesh, "physical_z");

            // top nodes (rows interface_row ... y_segments)
            for (int j = interface_row; j <= parameters.y_segments; ++j) {
                for (int i = 0; i <= parameters.x_segments; ++i) {
                    const auto & node = node_grid[j][i];
                    const auto coords = coord_system.coordinates(node->point());
                    const auto z = coords[1] + half_crack;
                    const auto num = top_field(top_space.node_map().at(node));
                    const auto ex = analytical_potential(parameters, conductivity_ratio, z);
                    num_mf(node) = num;
                    exact_mf(node) = ex;
                    err_mf(node) = std::abs(num - ex);
                    pz_mf(node) = z;
                }
            }

            // bottom interior nodes (rows 0 ... interface_row-1)
            for (int j = 0; j < interface_row; ++j) {
                for (int i = 0; i <= parameters.x_segments; ++i) {
                    const auto & node = node_grid[j][i];
                    const auto coords = coord_system.coordinates(node->point());
                    const auto z = coords[1] - half_crack;
                    const auto num = bottom_field(bottom_space.node_map().at(node));
                    const auto ex = analytical_potential(parameters, conductivity_ratio, z);
                    num_mf(node) = num;
                    exact_mf(node) = ex;
                    err_mf(node) = std::abs(num - ex);
                    pz_mf(node) = z;
                }
            }

            // duplicate interface nodes
            for (int i = 0; i <= parameters.x_segments; ++i) {
                const auto & orig = node_grid[interface_row][i];
                const auto & dup = dup_interface[i];
                const auto coords = coord_system.coordinates(orig->point());
                const auto z = coords[1] - half_crack;
                const auto num = bottom_field(bottom_space.node_map().at(orig));
                const auto ex = analytical_potential(parameters, conductivity_ratio, z);
                num_mf(dup) = num;
                exact_mf(dup) = ex;
                err_mf(dup) = std::abs(num - ex);
                pz_mf(dup) = z;
            }

            auto writer = mito::io::vtk::field_writer(
                output_options.label_prefix + "_ratio_" + tag, vtk_mesh, coord_system);
            writer.record(num_mf);
            writer.record(exact_mf);
            writer.record(err_mf);
            writer.record(pz_mf);
            writer.write();
        }
#endif

        return CaseResult{ conductivity_ratio, l2_error, max_profile_error, max_row_spread,
                           profile };
    }

}    // namespace hybrid_interface_chapter5_verification

// end of file
