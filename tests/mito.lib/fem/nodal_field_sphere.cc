// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>
#include <mito/mesh.h>
#include <mito/io.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Fem, NodalFieldSphere)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a sphere
    std::ifstream fileStream("sphere.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

    // a nodal field on the mesh
    auto nodal_field = mito::fem::nodal_field<mito::vector_t<3>, 3>(mesh, "normal");

    // the normal field to the submanifold
    constexpr auto normal_field = mito::fields::field([](const coordinates_t & x) -> auto {
        mito::scalar_t phi = std::atan2(x[1], x[0]);
        mito::scalar_t theta = std::atan2(std::hypot(x[1], x[0]), x[2]);
        return std::sin(theta) * std::cos(phi) * mito::e_0<3>
             + std::sin(theta) * std::sin(phi) * mito::e_1<3> + std::cos(theta) * mito::e_2<3>;
    });

    // fill information in nodal field
    for (auto & [node, value] : nodal_field) {
        // get the coordinates of the node
        auto & coordinates = coord_system.coordinates(node.point());
        // compute the value of the normal field at those coordinates
        value = normal_field(coordinates);
    }

#ifdef WITH_VTK
    // write mesh to vtk file
    auto writer = mito::io::vtk::writer("sphere_field", mesh, coord_system);
    // sign {nodal_field} up with the writer
    writer.record(nodal_field);
    // write output file
    writer.write();
#endif
}