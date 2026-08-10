// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using cartesian_coordinates_t = mito::geometry::cartesian<2>::coordinates_t;

// polar coordinates
using polar_coordinates_t = mito::geometry::polar::coordinates_t;


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

    // the metric space
    using metric_space_t = mito::geometry::euclidean_metric_space<coordT2>;

    // loop over the mesh cells
    auto area = 0.0;
    for (const auto & cell : mesh.cells()) {
        area += mito::geometry::volume(cell, coord_system_changed, metric_space_t::w);
    }

    // all done
    return area;
}


TEST(Mesh, Disk)
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
