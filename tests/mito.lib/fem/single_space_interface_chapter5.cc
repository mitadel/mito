// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>

#include <algorithm>
#include <cmath>
#include <vector>


namespace single_space_chapter5 {

    using scalar_t = mito::tensor::scalar_t;
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
    using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
    using triangle_t = mito::geometry::triangle_t<2>;
    using segment_t = mito::geometry::segment_t<2>;
    using node_t = mito::geometry::node_t<2>;

    constexpr int degree = 1;
    constexpr int bulk_doe = 2;

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
    };

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

    inline auto create_polyline_mesh(const std::vector<node_t> & nodes)
        -> mito::mesh::mesh_t<segment_t>
    {
        auto mesh = mito::mesh::mesh<segment_t>();
        for (std::size_t i = 0; i + 1 < nodes.size(); ++i) {
            mesh.insert({ nodes[i], nodes[i + 1] });
        }
        return mesh;
    }

    inline auto run_case(const Parameters & parameters, const scalar_t conductivity_ratio)
        -> scalar_t
    {
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        const auto domain_half_height = parameters.half_thickness - 0.5 * parameters.crack_opening;
        const auto dx = parameters.width / static_cast<scalar_t>(parameters.x_segments);
        const auto dy = 2.0 * domain_half_height / static_cast<scalar_t>(parameters.y_segments);
        const auto half_crack = 0.5 * parameters.crack_opening;
        const auto interface_row = parameters.y_segments / 2;

        // Build structured node grid (same geometry as the two-space driver)
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

        // duplicate mesh nodes at the interface row (the duplicates are for the negative side)
        auto neg_interface = std::vector<node_t>{};
        neg_interface.reserve(parameters.x_segments + 1);
        for (int i = 0; i <= parameters.x_segments; ++i) {
            const auto c = coord_system.coordinates(node_grid[interface_row][i]->point());
            neg_interface.push_back(mito::geometry::node(coord_system, { c[0], c[1] }));
        }

        // build a single triangle mesh covering the full domain
        auto mesh = mito::mesh::mesh<triangle_t>();

        // top half: rows interface_row ... y_segments-1
        for (int j = interface_row; j < parameters.y_segments; ++j) {
            for (int i = 0; i < parameters.x_segments; ++i) {
                const auto & n00 = node_grid[j][i];
                const auto & n10 = node_grid[j][i + 1];
                const auto & n01 = node_grid[j + 1][i];
                const auto & n11 = node_grid[j + 1][i + 1];

                if ((i + j) % 2 == 0) {
                    mesh.insert({ n00, n10, n11 });
                    mesh.insert({ n00, n11, n01 });
                } else {
                    mesh.insert({ n00, n10, n01 });
                    mesh.insert({ n10, n11, n01 });
                }
            }
        }

        // bottom half: rows 0 ... interface_row-1
        for (int j = 0; j < interface_row; ++j) {
            for (int i = 0; i < parameters.x_segments; ++i) {
                const auto & n00 = node_grid[j][i];
                const auto & n10 = node_grid[j][i + 1];
                const auto & n01 =
                    (j + 1 == interface_row) ? neg_interface[i] : node_grid[j + 1][i];
                const auto & n11 =
                    (j + 1 == interface_row) ? neg_interface[i + 1] : node_grid[j + 1][i + 1];

                if ((i + j) % 2 == 0) {
                    mesh.insert({ n00, n10, n11 });
                    mesh.insert({ n00, n11, n01 });
                } else {
                    mesh.insert({ n00, n10, n01 });
                    mesh.insert({ n10, n11, n01 });
                }
            }
        }

        // boundary meshes for Dirichlet BCs (outer boundaries only)
        auto top_boundary = create_polyline_mesh(node_grid.back());
        auto bottom_boundary = create_polyline_mesh(node_grid.front());

        // manifold on the single combined mesh
        auto manifold = mito::manifolds::manifold(mesh, coord_system);

        using bulk_element_t = mito::fem::isoparametric_simplex_t<degree, decltype(manifold)>;

        // dirichlet values
        auto dirichlet_values =
            mito::functions::function([parameters](const coordinates_t & coords) -> scalar_t {
                return coords[1] < 0.0 ? parameters.phi_bottom : parameters.phi_top;
            });

        // Combine both boundaries into one constraint set for the single space.
        // We merge top and bottom boundary meshes into a single polyline mesh
        auto boundary_mesh = mito::mesh::mesh<segment_t>();
        for (const auto & cell : top_boundary.cells()) {
            boundary_mesh.insert(cell.nodes());
        }
        for (const auto & cell : bottom_boundary.cells()) {
            boundary_mesh.insert(cell.nodes());
        }
        auto bc = mito::constraints::dirichlet_bc(boundary_mesh, dirichlet_values);

        // one function space on the combined mesh
        auto space = mito::fem::function_space<bulk_element_t>(manifold, bc);

        // bulk weakform
        auto bulk_lhs =
            mito::fem::blocks::grad_grad_block<bulk_element_t, bulk_quadrature_rule_t>();
        auto weakform = mito::fem::weakform<bulk_element_t>();
        weakform.add_block(bulk_lhs);

        // discrete system with a single contribution
        auto discrete_system = mito::fem::discrete_system<linear_system_t>(
            "single_space_chapter5",
            mito::fem::Contribution{ space, weakform, parameters.kappa_se });

        // prescribe Dirichlet values
        for (const auto & constrained_node : space.constrained_nodes()) {
            for (const auto & [mesh_node, disc_node] : space.node_map()) {
                if (disc_node == constrained_node) {
                    discrete_system.set_prescribed_value(
                        constrained_node,
                        dirichlet_values(coord_system.coordinates(mesh_node->point())));
                    break;
                }
            }
        }

        discrete_system.assemble();

        // interface coupling
        const auto kappa_m = conductivity_ratio * parameters.kappa_se;
        const auto crack_coeff = kappa_m / parameters.crack_opening;
        const auto diff_coeff = parameters.crack_opening * kappa_m / 4.0;

        auto iface_sample = mito::fem::make_embedded_interface_segment_element(
            coord_system, space, node_grid[interface_row][0], node_grid[interface_row][1]);
        using interface_element_t = decltype(iface_sample);

        mito::fem::blocks::InterfaceJumpJumpMassBlock<bulk_element_t, interface_element_t> jj_block(
            crack_coeff);
        mito::fem::blocks::InterfaceAverageAverageGradBlock<bulk_element_t, interface_element_t>
            aa_block(diff_coeff);
        mito::fem::InterfaceWeakform<bulk_element_t, interface_element_t, 4> iface_wf;
        iface_wf.add_block(jj_block);
        iface_wf.add_block(aa_block);

        // positive-side nodes = original interface row, negative-side = duplicated nodes
        auto positive_nodes = std::vector<node_t>{};
        positive_nodes.reserve(static_cast<std::size_t>(parameters.x_segments + 1));
        for (int i = 0; i <= parameters.x_segments; ++i) {
            positive_nodes.push_back(node_grid[interface_row][i]);
        }

        auto pairings = mito::fem::build_interface_pairings(
            space, positive_nodes, neg_interface, parameters.x_segments);

        mito::fem::assemble_interface(
            discrete_system, coord_system, space, iface_wf, 1.0, pairings);

        auto ksp = matrix_solver_t(discrete_system.linear_system());
        ksp.create();
        ksp.set_options("-ksp_type preonly -pc_type lu");
        ksp.solve();

        discrete_system.read_solution();
        ksp.destroy();
        const auto & global_solution = discrete_system.solution();

        // project solution into a fem field for L2 norm computation
        auto top_field = space.template fem_field<scalar_t>("top_solution");
        auto bottom_field = space.template fem_field<scalar_t>("bottom_solution");

        for (const auto & [mesh_node, disc_node] : space.node_map()) {
            top_field(disc_node) = global_solution(disc_node);
            bottom_field(disc_node) = global_solution(disc_node);
        }

        // L2 error: evaluate by extracting through-thickness profile at mid-column
        const auto mid_column = parameters.x_segments / 2;
        scalar_t max_profile_error = 0.0;

        // bottom half: physical z = y - half_crack
        for (int j = 0; j <= interface_row; ++j) {
            const auto & mid_node =
                (j == interface_row) ? neg_interface[mid_column] : node_grid[j][mid_column];
            const auto coords = coord_system.coordinates(mid_node->point());
            const auto z = coords[1] - half_crack;
            const auto numerical = global_solution(space.node_map().at(mid_node));
            const auto exact = analytical_potential(parameters, conductivity_ratio, z);
            max_profile_error = std::max(max_profile_error, std::abs(numerical - exact));
        }

        // top half: physical z = y + half_crack
        for (int j = interface_row; j <= parameters.y_segments; ++j) {
            const auto & mid_node = node_grid[j][mid_column];
            const auto coords = coord_system.coordinates(mid_node->point());
            const auto z = coords[1] + half_crack;
            const auto numerical = global_solution(space.node_map().at(mid_node));
            const auto exact = analytical_potential(parameters, conductivity_ratio, z);
            max_profile_error = std::max(max_profile_error, std::abs(numerical - exact));
        }

        return max_profile_error;
    }

}    // namespace single_space_chapter5


TEST(Fem, SingleSpaceInterfaceChapter5Verification)
{
    constexpr auto tol_profile = 1e-8;

    mito::petsc::initialize();

    const auto parameters = single_space_chapter5::Parameters{};

    for (const auto ratio : { 1.0, 2.0, 1000.0, 0.5, 0.001 }) {
        const auto max_error = single_space_chapter5::run_case(parameters, ratio);

        EXPECT_LT(max_error, tol_profile)
            << "ratio=" << ratio << " max_profile_error=" << max_error;
    }

    mito::petsc::finalize();
}
