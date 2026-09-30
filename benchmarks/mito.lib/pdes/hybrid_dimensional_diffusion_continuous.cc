// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// accuracy benchmark for hybrid-dimensional diffusion with a continuous interface potential:
// solve the problem verified in tests/mito.lib/fem/hybrid_dimensional_diffusion_continuous.cc
// and write the bulk and interface potentials to VTK and to a profile csv

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

// a mid-column sample of the potential profile along y, for plotting against physical_z
struct ProfileSample {
    scalar_t reduced_y;
    scalar_t physical_z;
    scalar_t numerical;
    scalar_t exact;
    scalar_t absolute_error;
};

// write the potential profile to a csv, sorted by physical_z, for plot.py to read
auto
write_profile_csv(const std::string & tag, const std::vector<ProfileSample> & profile) -> void
{
    std::ofstream stream("hybrid_dimensional_diffusion_continuous_ratio_" + tag + ".csv");
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
    journal::info_t channel("apps.hybrid_dimensional_diffusion_continuous");

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

    // the unit diffusivity, scaled per-contribution below by the physical conductivities
    auto identity = mito::functions::identity<coordinates_t, 2>();

    // the bulk weakform: diffusion scaled by kappa_se, with no source
    auto bulk_lhs =
        parameters.kappa_se * mito::fem::blocks::diffusion<bulk_element_t, doe>(identity);
    auto bulk_rhs = mito::fem::blocks::source<bulk_element_t, doe>(zero);


    // the interface weakform: tangential diffusion along the crack, scaled by the collapsed
    // layer's conductance -w * kappa_m, with no source
    const auto kappa_m = conductivity_ratio * parameters.kappa_se;
    auto interface_lhs = (-parameters.layer_thickness * kappa_m)
                       * mito::fem::blocks::diffusion<interface_element_t, doe>(identity);
    auto interface_rhs = mito::fem::blocks::source<interface_element_t, doe>(zero);


    // one coupled weakform: bulk diffusion, plus the tangential diffusion along the crack
    auto weakform = mito::fem::coupled_weakform(
        mito::fem::term(bulk_space, bulk_lhs), mito::fem::term(bulk_space, bulk_rhs),
        mito::fem::term(interface_space, interface_lhs),
        mito::fem::term(interface_space, interface_rhs));

    // the discrete system
    auto discrete_system =
        mito::fem::discrete_system<linear_system_t>("hybrid_dimensional_diffusion", weakform);

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
    channel << "ratio=" << conductivity_ratio << ", L2 error=" << l2_error << journal::endl;

    const auto tag = ratio_tag(conductivity_ratio);

    // sample the numerical solution on the mid column, against the physical z coordinate (which
    // has a gap at the layer), for the profile plot
    const auto mid_column = parameters.x_segments / 2;
    auto profile = std::vector<ProfileSample>{};
    profile.reserve(parameters.y_segments);
    for (int j = 0; j <= parameters.y_segments; ++j) {
        // the interface row has no counterpart in the exact layered solution
        if (j == interface_row) {
            continue;
        }
        const auto & node = node_grid[j][mid_column];
        const auto coordinates = coord_system.coordinates(node->point());
        const auto numerical = solution(bulk_space.node_map().at(node));
        const auto physical_z = physical_z_from_reduced_y(parameters, coordinates[1]);
        const auto exact = analytical_potential(parameters, conductivity_ratio, physical_z);
        profile.push_back(
            { coordinates[1], physical_z, numerical, exact, std::abs(numerical - exact) });
    }
    std::sort(profile.begin(), profile.end(), [](const auto & a, const auto & b) {
        return a.physical_z < b.physical_z;
    });
    write_profile_csv(tag, profile);

#ifdef WITH_VTK
    // the exact potential and the pointwise error, both keyed by discretization node
    auto exact_field = bulk_space.fem_field<scalar_t>("exact potential");
    auto error_field = bulk_space.fem_field<scalar_t>("error");
    for (const auto & row : node_grid) {
        for (const auto & node : row) {
            const auto & discretization_node = bulk_space.node_map().at(node);
            const auto coordinates = coord_system.coordinates(node->point());
            const auto exact = exact_projected(coordinates);
            exact_field(discretization_node) = exact;
            error_field(discretization_node) = solution(discretization_node) - exact;
        }
    }

    // write the bulk potential (numerical, exact, error) to vtk
    auto bulk_writer = mito::io::vtk::field_writer(
        "hybrid_dimensional_diffusion_continuous_bulk_ratio_" + tag, bulk_space, coord_system);
    bulk_writer.record(solution.nodal_values(), "numerical potential");
    bulk_writer.record(exact_field.nodal_values());
    bulk_writer.record(error_field.nodal_values());
    bulk_writer.write();

    // write the interface potential to vtk
    auto interface_writer = mito::io::vtk::field_writer(
        "hybrid_dimensional_diffusion_continuous_interface_ratio_" + tag, interface_space,
        coord_system);
    interface_writer.record(solution.nodal_values(), "numerical potential");
    interface_writer.write();

    channel << "wrote vtk output for ratio=" << conductivity_ratio << journal::endl;
#endif

    // all done
    return;
}


int
main()
{
    // initialize PETSc
    mito::petsc::initialize();

    const auto parameters = Parameters{};

    // the conductivity ratios verified in the regression test
    constexpr auto ratios = std::array<scalar_t, 5>{ 0.001, 0.5, 1.0, 2.0, 1000.0 };

    for (const auto ratio : ratios) {
        run_case(parameters, ratio);
    }

    // finalize PETSc
    mito::petsc::finalize();

    return 0;
}


// end of file
