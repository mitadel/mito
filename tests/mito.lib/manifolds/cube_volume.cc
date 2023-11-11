#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>
#include <mito/io.h>


// the basis for vector fields
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


// compute the volume of a tetrahedron via the metric volume element
auto
volume_form(
    const auto & w, const mito::geometry::geometry_t<3> & geometry,
    const mito::topology::tetrahedron_t & tetrahedron) -> mito::scalar_t
{
    // get the directors of the tetrahedron
    auto directors = geometry.directors(tetrahedron);

    // compute volume of tetrahedron
    auto volume = 1. / 6. * w(directors[0], directors[1], directors[2]);

    // all done
    return volume;
}


// compute the volume of a tetrahedron as the determinant of the matrix of its vertices
auto
volume_determinant(
    mito::geometry::geometry_t<3> & geometry, const mito::topology::tetrahedron_t & tetrahedron)
    -> mito::scalar_t
{
    // number of element vertices
    constexpr int D = 3;
    constexpr int V = D + 1;

    // a container to store the coordinates of each vertex in a tensor
    mito::matrix_t<V> pointsTensor;

    // collect element vertices
    // TOFIX: when the vertices will be returned in order, perhaps we can remove the {fabs} below
    auto element_vertices = tetrahedron->vertices();

    // assert you found V element vertices
    assert(V == std::size(element_vertices));

    // loop on element vertices
    int v = 0;
    for (const auto & vertex : element_vertices) {
        // fill up pointsTensor container
        for (int d = 0; d < D; ++d) {
            pointsTensor[v * V + d] = geometry.point(vertex)->coordinates()[d];
        }
        pointsTensor[v * V + D] = 1.0;
        // update element vertices counter
        ++v;
    }

    // compute the volume of the e-th element
    auto volume = std::fabs(pyre::tensor::determinant(pointsTensor)) / pyre::tensor::factorial<D>();

    // all done
    return volume;
}

TEST(Manifolds, CubeVolume)
{
    // the basis one-forms
    constexpr auto dx = mito::manifolds::one_form(e_x);
    constexpr auto dy = mito::manifolds::one_form(e_y);
    constexpr auto dz = mito::manifolds::one_form(e_z);

    // the metric volume element
    constexpr auto w = mito::manifolds::wedge(dx, dy, dz);

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::io::summit::reader<mito::topology::tetrahedron_t>(fileStream, geometry);

    mito::scalar_t volume_new = 0.0;
    mito::scalar_t volume_old = 0.0;
    for (const auto & cell : mesh.cells()) {
        volume_new += volume_form(w, geometry, cell);
        volume_old += volume_determinant(geometry, cell);
    }
    // std::cout << volume_new << "\t" << volume_old << std::endl;
    EXPECT_NEAR(volume_new, volume_old, 1.e-15);
}


// end of file
