// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <mito.h>


// cartesian coordinates in 2D
using cartesian_coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// polar coordinates in 2D
using polar_coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;

// simplicial cells in 2D
using cell_t = mito::geometry::triangle_t<2>;

// the r scalar field in 2D
constexpr auto r = mito::functions::component<polar_coordinates_t, 0>;
// the theta scalar field in 2D
constexpr auto theta = mito::functions::component<polar_coordinates_t, 1>;


int
main()
{
    // initialize PETSc
    mito::petsc::initialize();

    // make a channel
    journal::info_t channel("benchmark.laplacian_hollow_disk");

    // the cartesian coordinate system
    auto cartesian_coord_system = mito::geometry::coordinate_system<cartesian_coordinates_t>();

    // read the mesh of a hollow half disk in 2D
    std::ifstream fileStream("hollow_half_disk.summit");
    auto mesh = mito::io::summit::reader<cell_t>(fileStream, cartesian_coord_system);

    // perform change of coordinates from cartesian to spherical
    auto polar_coord_system =
        mito::geometry::coordinate_system<polar_coordinates_t>(cartesian_coord_system);

    // create the body manifold
    auto manifold = mito::manifolds::manifold(mesh, polar_coord_system);

    // compute the area of the hollow half disk
    auto area = manifold.volume();

    // exact area is 15/32*pi
    auto exact_area = 15.0 / 32.0 * std::numbers::pi;

    // report
    channel << "Area of the hollow half disk: " << area << "\t" << exact_area << journal::endl;

    // get the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the zero field
    auto zero = mito::functions::zero<polar_coordinates_t>;

    // set homogeneous Dirichlet boundary condition
    auto constraints = mito::constraints::dirichlet_bc(boundary_mesh, zero);

    // the manufactured solution field
    auto u_ex = (r - 1.0) * (r - 0.25) * r * r * r * mito::functions::sin(3.0 * theta);

    // compute the right hand side
    auto f = -mito::fields::divergence(mito::fields::gradient(u_ex));

#ifdef WITH_VTK
    // the forcing term mesh field on the mesh (for visualization)
    auto forcing = mito::discrete::mesh_field(mesh, polar_coord_system, f, "forcing term");
    // the exact solution mesh field on the mesh (for visualization)
    auto exact_solution =
        mito::discrete::mesh_field(mesh, polar_coord_system, u_ex, "exact solution");
    // write mesh to vtk file
    auto writer = mito::io::vtk::field_writer("laplacian_hollow_disk", mesh, polar_coord_system);
    // sign {forcing} up with the writer
    writer.record(forcing);
    // sign {exact_solution} up with the writer
    writer.record(exact_solution);
    // write output file
    writer.write();
#endif

    // finalize PETSc
    mito::petsc::finalize();
}

// end of file
