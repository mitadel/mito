// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


template <mito::geometry::CoordinateType coordT>
auto
area(std::string mesh_file_name) -> mito::scalar_t
{
    // a {coordT} coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, coordT>();

    // read the mesh
    auto filestream = std::ifstream(mesh_file_name);
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(filestream, coord_system);

    // create a manifold on {mesh}
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // compute the area of the manifold
    auto area = manifold.volume();

    // all done
    return area;
}


TEST(Manifolds, Disk)
{
    // compute the area of the disk parametrized in polar coordinates
    auto area_polar = area<mito::geometry::POLAR>("disk_polar.summit");

    // compute the area of the disk parametrized in cartesian coordinates
    auto area_cartesian = area<mito::geometry::CARTESIAN>("disk_cartesian.summit");

    // expect to get the same result
    EXPECT_DOUBLE_EQ(area_cartesian, area_polar);

    // exact area: the area of a disk with unit radius is {pi}
    auto exact = std::numbers::pi;

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the disk)
    EXPECT_NEAR(exact, area_cartesian, 0.01);
}

// end of file
