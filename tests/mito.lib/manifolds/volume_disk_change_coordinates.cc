// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/manifolds.h>


// cartesian coordinates in 2D
using cartesian_coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// polar coordinates
using polar_coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;


template <mito::geometry::coordinates_c coordT1, mito::geometry::coordinates_c coordT2>
auto
area_change_coordinates(std::string mesh_file_name) -> mito::tensor::scalar_t
{
    // a {coordT1} coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordT1>();

    // read the mesh in its native coordinates system {coordT1}
    auto filestream = std::ifstream(mesh_file_name);
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(filestream, coord_system);

    // perform change of coordinates from {coordT1} to {coordT2}
    auto coord_system_changed = mito::geometry::coordinate_system<coordT2>(coord_system);

    // create a manifold on {mesh} with the coordinate system {coordT2}
    auto manifold = mito::manifolds::manifold(mesh, coord_system_changed);

    // compute the area of the manifold
    auto area = manifold.volume();

    // all done
    return area;
}


TEST(Manifolds, Disk)
{
    // compute the area in polar coordinates on a cartesian mesh
    auto area_polar = area_change_coordinates<cartesian_coordinates_t, polar_coordinates_t>(
        "disk_cartesian.summit");

    // compute the area in polar coordinates on a cartesian mesh
    auto area_cartesian =
        area_change_coordinates<polar_coordinates_t, cartesian_coordinates_t>("disk_polar.summit");

    // expect to get the same result
    EXPECT_DOUBLE_EQ(area_cartesian, area_polar);

    // exact area: the area of a disk with unit radius is {pi}
    auto exact = std::numbers::pi;

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the disk)
    EXPECT_NEAR(exact, area_polar, 0.01);
}

// end of file
