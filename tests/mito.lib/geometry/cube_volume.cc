// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <mito/io.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;

// the basis vectors
static constexpr auto e_x = mito::tensor::e_0<3>;
static constexpr auto e_y = mito::tensor::e_1<3>;
static constexpr auto e_z = mito::tensor::e_2<3>;


// compute the volume of a tetrahedron via the metric volume element
auto
volume_form(
    const auto & w, const mito::geometry::coordinate_system_t<coordinates_t> & coord_system,
    const mito::geometry::tetrahedron_t<3> & tetrahedron) -> mito::tensor::scalar_t
{
    // get the directors of the tetrahedron
    auto [_, directors] = mito::geometry::directors(tetrahedron, coord_system);

    // compute volume of tetrahedron
    auto volume = 1. / 6. * w(directors[0], directors[1], directors[2]);

    // all done
    return volume;
}


// compute the volume of a tetrahedron as the determinant of the matrix of its vertices
auto
volume_determinant(
    const mito::geometry::coordinate_system_t<coordinates_t> & coord_system,
    const mito::geometry::tetrahedron_t<3> & tetrahedron) -> mito::tensor::scalar_t
{
    // number of element vertices
    constexpr int D = 3;
    constexpr int V = D + 1;

    // a container to store the coordinates of each vertex in a tensor
    mito::tensor::matrix_t<V> pointsTensor;

    // collect element nodes
    auto element_nodes = tetrahedron.nodes();

    // assert you found V element vertices
    assert(V == std::size(element_nodes));

    // loop on element vertices
    int v = 0;
    for (const auto & node : element_nodes) {
        // fill up pointsTensor container
        for (int d = 0; d < D; ++d) {
            pointsTensor[v * V + d] = coord_system.coordinates(node->point())[d];
        }
        pointsTensor[v * V + D] = 1.0;
        // update element vertices counter
        ++v;
    }

    // compute the volume of the e-th element
    auto volume = std::fabs(mito::tensor::determinant(pointsTensor)) / mito::tensor::factorial<D>();

    // all done
    return volume;
}

TEST(Tensor, CubeVolume)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);
    constexpr auto dz = mito::tensor::one_form(e_z);

    // the metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy, dz);

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    auto volume_new = 0.0;
    auto volume_old = 0.0;
    for (const auto & cell : mesh.cells()) {
        volume_new += volume_form(w, coord_system, cell);
        volume_old += volume_determinant(coord_system, cell);
    }

    // check that the two calculations led to the same result
    EXPECT_DOUBLE_EQ(volume_new, volume_old);
}


// end of file
