// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// accuracy benchmark for hybrid-dimensional diffusion with a discontinuous interface potential:
// solve the problem verified in tests/mito.lib/fem/hybrid_dimensional_diffusion_discontinuous.cc
// and write the potential of either half-domain to VTK and to a profile csv

#include <mito.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>


using scalar_t = mito::tensor::scalar_t;
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
using triangle_t = mito::geometry::triangle_t<2>;
using segment_t = mito::geometry::segment_t<2>;
using node_t = mito::geometry::node_t<2>;

// first degree finite elements for the bulk (triangles) and the interface (embedded segments)
using bulk_element_t = mito::fem::finite_element_family<triangle_t, 1>;
using interface_element_t = mito::fem::finite_element_family<segment_t, 1>;

// the traits of the butterfly elements built on the interface
using interface_traits_t = mito::fem::InterfaceElementTraits<interface_element_t>;

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

// a filesystem-friendly tag for a conductivity ratio, e.g. 0.001 -> "0p001"
auto
ratio_tag(scalar_t ratio) -> std::string
{
    std::ostringstream stream;
    stream << ratio;
    auto tag = stream.str();
    std::replace(tag.begin(), tag.end(), '.', 'p');
    return tag;
}

// one sample of the potential profile along the mid column
struct ProfileSample {
    scalar_t reduced_y;
    scalar_t physical_z;
    scalar_t numerical;
    scalar_t exact;
    scalar_t absolute_error;
};

// write the profile in the format {plot.py} reads
auto
write_profile_csv(const std::string & tag, const std::vector<ProfileSample> & profile) -> void
{
    std::ofstream stream("hybrid_dimensional_diffusion_discontinuous_ratio_" + tag + ".csv");
    stream << "reduced_y,physical_z,numerical,exact,absolute_error\n";
    stream << std::setprecision(16);
    for (const auto & sample : profile) {
        stream << sample.reduced_y << ',' << sample.physical_z << ',' << sample.numerical << ','
               << sample.exact << ',' << sample.absolute_error << '\n';
    }
}

auto
run_case(const Parameters & parameters, scalar_t conductivity_ratio) -> void
{
    journal::info_t channel("apps.hybrid_dimensional_diffusion_discontinuous");

    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    const auto domain_half_height = reduced_half_thickness(parameters);
    const auto dx = parameters.width / static_cast<scalar_t>(parameters.x_segments);
    const auto dy = 2.0 * domain_half_height / static_cast<scalar_t>(parameters.y_segments);
    const auto half_layer = 0.5 * parameters.layer_thickness;

    // the shared node grid: both half-domains touch y = 0, but get independent discretization
    // nodes there since their function spaces are built without a shared node map
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

    // the interface row splits the mesh into a top and a bottom half, each meshed separately
    assert(parameters.y_segments % 2 == 0);
    const auto interface_row = parameters.y_segments / 2;

    // two triangles per structured cell, with alternating diagonals
    auto top_mesh = mito::mesh::mesh<triangle_t>();
    for (int j = interface_row; j < parameters.y_segments; ++j) {
        for (int i = 0; i < parameters.x_segments; ++i) {
            const auto & node_00 = node_grid[j][i];
            const auto & node_10 = node_grid[j][i + 1];
            const auto & node_01 = node_grid[j + 1][i];
            const auto & node_11 = node_grid[j + 1][i + 1];
            if ((i + j) % 2 == 0) {
                top_mesh.insert({ node_00, node_10, node_11 });
                top_mesh.insert({ node_00, node_11, node_01 });
            } else {
                top_mesh.insert({ node_00, node_10, node_01 });
                top_mesh.insert({ node_10, node_11, node_01 });
            }
        }
    }
    auto bottom_mesh = mito::mesh::mesh<triangle_t>();
    for (int j = 0; j < interface_row; ++j) {
        for (int i = 0; i < parameters.x_segments; ++i) {
            const auto & node_00 = node_grid[j][i];
            const auto & node_10 = node_grid[j][i + 1];
            const auto & node_01 = node_grid[j + 1][i];
            const auto & node_11 = node_grid[j + 1][i + 1];
            if ((i + j) % 2 == 0) {
                bottom_mesh.insert({ node_00, node_10, node_11 });
                bottom_mesh.insert({ node_00, node_11, node_01 });
            } else {
                bottom_mesh.insert({ node_00, node_10, node_01 });
                bottom_mesh.insert({ node_10, node_11, node_01 });
            }
        }
    }

    // the outer Dirichlet boundaries: the top mesh's top row, the bottom mesh's bottom row
    auto top_boundary = mito::mesh::mesh<segment_t>();
    auto bottom_boundary = mito::mesh::mesh<segment_t>();
    for (int i = 0; i < parameters.x_segments; ++i) {
        top_boundary.insert({ node_grid.back()[i], node_grid.back()[i + 1] });
        bottom_boundary.insert({ node_grid.front()[i], node_grid.front()[i + 1] });
    }

    // the interface mesh on the layer line y = 0, which carries the geometry of the crack
    auto interface_mesh = mito::mesh::mesh<segment_t>();
    for (int i = 0; i < parameters.x_segments; ++i) {
        interface_mesh.insert({ node_grid[interface_row][i], node_grid[interface_row][i + 1] });
    }

    // the manifolds
    auto top_manifold = mito::manifolds::manifold(top_mesh, coord_system, metric_space_t::w);
    auto bottom_manifold = mito::manifolds::manifold(bottom_mesh, coord_system, metric_space_t::w);
    // the segments run in +x, so positive orientation needs the normal to be the tangent rotated
    // clockwise (w(normal, tangent) > 0)
    auto normal = mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, -1.0 });
    auto interface_manifold = mito::manifolds::submanifold(interface_mesh, coord_system, normal);

    // the prescribed potentials on the bottom and top boundaries
    auto dirichlet_values =
        mito::functions::function([parameters](const coordinates_t & coordinates) -> scalar_t {
            return coordinates[1] < 0.0 ? parameters.phi_bottom : parameters.phi_top;
        });
    auto top_constraints = mito::constraints::dirichlet_bc(top_boundary, dirichlet_values);
    auto bottom_constraints = mito::constraints::dirichlet_bc(bottom_boundary, dirichlet_values);

    // two independent bulk function spaces: no shared node map, so the interface row gets
    // independent discretization nodes on each side
    auto top_space = mito::fem::function_space<bulk_element_t>(top_manifold, top_constraints);
    auto bottom_space =
        mito::fem::function_space<bulk_element_t>(bottom_manifold, bottom_constraints);

    // the butterfly elements on the interface: walking along the tangent, which runs in +x, the
    // top half of the domain is on the left and the bottom half on the right
    auto interface = mito::fem::interface_elements<interface_element_t>(
        interface_manifold, top_space.node_map(), bottom_space.node_map());

    // the unit diffusivity, scaled per-term below by the physical conductivities
    auto identity = mito::functions::identity<coordinates_t, 2>();
    auto one = mito::functions::one<coordinates_t>;

    // the transverse and tangential conductances of the crack; the average-average block already
    // carries a factor 1/4 internally, so its coefficient is -w * kappa_m without the /4
    const auto kappa_m = conductivity_ratio * parameters.kappa_se;
    const auto jump_jump_coefficient = kappa_m / parameters.layer_thickness;
    const auto average_average_coefficient = -parameters.layer_thickness * kappa_m;

    // one coupled weakform: bulk diffusion on either half, plus the two crack terms
    auto weakform = mito::fem::coupled_weakform(
        mito::fem::term(
            top_space,
            parameters.kappa_se * mito::fem::blocks::diffusion<bulk_element_t, doe>(identity)),
        mito::fem::term(
            bottom_space,
            parameters.kappa_se * mito::fem::blocks::diffusion<bulk_element_t, doe>(identity)),
        mito::fem::term(
            interface,
            jump_jump_coefficient * mito::fem::blocks::jump_jump<interface_traits_t, doe>(one)),
        mito::fem::term(
            interface,
            average_average_coefficient
                * mito::fem::blocks::average_average_gradient<interface_traits_t, doe>(identity)));

    // the discrete system
    auto discrete_system = mito::fem::discrete_system<linear_system_t>(
        "hybrid_dimensional_diffusion_discontinuous", weakform);

    // solve
    auto solver = mito::solvers::linear_solver<matrix_solver_t>(discrete_system);
    solver.set_options("-ksp_type preonly -pc_type lu");
    solver.solve();
    solver.destroy();

    const auto & solution = discrete_system.solution();

    // the analytical potential on each half; each side's own y = 0 row maps to its own crack
    // boundary, offset by the layer half-thickness
    auto exact_top = mito::functions::function(
        [parameters, conductivity_ratio,
         half_layer](const coordinates_t & coordinates) -> scalar_t {
            return analytical_potential(
                parameters, conductivity_ratio, coordinates[1] + half_layer);
        });
    auto exact_bottom = mito::functions::function(
        [parameters, conductivity_ratio,
         half_layer](const coordinates_t & coordinates) -> scalar_t {
            return analytical_potential(
                parameters, conductivity_ratio, coordinates[1] - half_layer);
        });

    // the L2 norm of the error over each half, combined
    const auto l2_top = mito::fem::compute_l2_norm<doe>(top_space, solution, exact_top);
    const auto l2_bottom = mito::fem::compute_l2_norm<doe>(bottom_space, solution, exact_bottom);
    const auto l2_error = std::sqrt(l2_top * l2_top + l2_bottom * l2_bottom);

    const auto tag = ratio_tag(conductivity_ratio);

    // sample the potential along the mid column; the interface row appears twice, once per side,
    // which is what makes the jump visible
    const auto mid_column = parameters.x_segments / 2;
    auto profile = std::vector<ProfileSample>();
    auto sample = [&](const auto & space, int j, scalar_t offset) {
        const auto & node = node_grid[j][mid_column];
        const auto y = coord_system.coordinates(node->point())[1];
        const auto z = y + offset;
        const auto numerical = solution(space.node_map().at(node));
        const auto exact = analytical_potential(parameters, conductivity_ratio, z);
        profile.push_back({ y, z, numerical, exact, std::abs(numerical - exact) });
    };
    for (int j = 0; j <= interface_row; ++j) {
        sample(bottom_space, j, -half_layer);
    }
    for (int j = interface_row; j <= parameters.y_segments; ++j) {
        sample(top_space, j, half_layer);
    }
    std::sort(profile.begin(), profile.end(), [](const auto & a, const auto & b) {
        return a.physical_z < b.physical_z;
    });
    write_profile_csv(tag, profile);

    // the jump in potential across the crack, read off the two faces at the mid column
    const auto & face_node = node_grid[interface_row][mid_column];
    const auto jump = solution(top_space.node_map().at(face_node))
                    - solution(bottom_space.node_map().at(face_node));
    channel << "ratio=" << conductivity_ratio << ", L2 error=" << l2_error
            << ", jump across the crack=" << jump << journal::endl;

#ifdef WITH_VTK
    // the exact potential and the pointwise error on either half
    auto write_half = [&](const auto & space, const auto & exact_field_function,
                          const std::string & side, int first_row, int last_row) {
        auto exact_field = space.template fem_field<scalar_t>("exact potential");
        auto error_field = space.template fem_field<scalar_t>("error");
        for (int j = first_row; j <= last_row; ++j) {
            for (const auto & node : node_grid[j]) {
                const auto & discretization_node = space.node_map().at(node);
                const auto exact = exact_field_function(coord_system.coordinates(node->point()));
                exact_field(discretization_node) = exact;
                error_field(discretization_node) = solution(discretization_node) - exact;
            }
        }
        auto writer = mito::io::vtk::field_writer(
            "hybrid_dimensional_diffusion_discontinuous_" + side + "_ratio_" + tag, space,
            coord_system);
        writer.record(solution.nodal_values(), "numerical potential");
        writer.record(exact_field.nodal_values(), "exact potential");
        writer.record(error_field.nodal_values(), "error");
        writer.write();
    };
    write_half(bottom_space, exact_bottom, "bottom", 0, interface_row);
    write_half(top_space, exact_top, "top", interface_row, parameters.y_segments);

    channel << "wrote VTK output for ratio=" << conductivity_ratio << journal::endl;
#endif

    // all done
    return;
}


int
main()
{
    mito::petsc::initialize();

    const auto parameters = Parameters{};
    constexpr auto ratios = std::array<scalar_t, 5>{ 0.001, 0.5, 1.0, 2.0, 1000.0 };

    for (const auto ratio : ratios) {
        run_case(parameters, ratio);
    }

    mito::petsc::finalize();

    return 0;
}


// end of file
