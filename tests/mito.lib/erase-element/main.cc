#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(EraseElement, DISABLED_TestEraseElement)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // load mesh of the unit square
    std::ifstream fileStream("small-square.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);

    // show me the points
    // std::cout << "Point cloud: " << std::endl;
    // std::cout << mesh.vertices() << std::endl;

    // assert you read 4 cells
    EXPECT_EQ(mesh.cells<2>().size(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mito::mesh::boundary<1>(mesh, topology).size(), 4);

    // show me the cells
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>())
    // {
    //     std::cout << *simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    for (const auto & simplex : mesh.cells<2>()) {
        // std::cout << *simplex << std::endl;
        mesh.erase(simplex);
        break;
    }

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>()) {
    //     std::cout << *simplex << std::endl;
    // }

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mito::mesh::boundary<1>(mesh, topology).size(), 5);

    // show me the boundary cells
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mito::mesh::boundary<1>(mesh, topology)) {
    //     std::cout << *simplex << std::endl;
    // }
}

template <int D, class... Args>
auto
instantiate_simplex(Args &&... args)
    -> mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>>
requires(
    (D > 0 && sizeof...(Args) == D + 1)
    && (std::is_same_v<Args, mito::topology::simplex_t<D - 1> &> && ...))
{
    mito::topology::unoriented_simplex_t<D> * resource =
        new mito::topology::unoriented_simplex_t<D>({ args... });
    return mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>>(resource, nullptr);
}

template <int D>
auto
instantiate_oriented_simplex(
    mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>> simplex, bool orientation)
    -> mito::utilities::shared_ptr<mito::topology::oriented_simplex_t<D>>
requires(D > 0)
{
    mito::topology::oriented_simplex_t<D> * resource =
        new mito::topology::oriented_simplex_t<D>(simplex, orientation);
    return mito::utilities::shared_ptr<mito::topology::oriented_simplex_t<D>>(resource, nullptr);
}

template <int D>
auto
instantiate_oriented_simplex() -> mito::utilities::shared_ptr<mito::topology::oriented_simplex_t<0>>
requires(D == 0)
{
    mito::topology::oriented_simplex_t<0> * resource = new mito::topology::oriented_simplex_t<0>();
    return mito::utilities::shared_ptr<mito::topology::oriented_simplex_t<0>>(resource, nullptr);
}

template <int D>
auto
erase(mito::topology::simplex_t<D> & simplex) -> void
requires(D == 0)
{
    simplex->erase();
    return;
}

template <int D>
auto
erase(mito::topology::simplex_t<D> & simplex) -> void
requires(D > 0)
{
    // grab a copy of the footprint
    auto footprint = simplex->footprint();

    // grab a copy of the composition
    auto composition = simplex->composition();

    // reset the simplex
    simplex->erase();

    // if this simplex is the last one using the footprint
    if (footprint.references() == 2) {
        // reset it
        footprint->erase();
    }

    // loop on subsimplices
    for (auto & subsimplex : composition) {
        // if this simplex is the last one using the subsimplex
        if (subsimplex.references() == 2) {
            erase(subsimplex);
        }
    }

    return;
}

TEST(EraseElement, TestEraseElement)
{
    // instantiate vertices
    auto vertex_0 = instantiate_oriented_simplex<0>();
    auto vertex_1 = instantiate_oriented_simplex<0>();
    auto vertex_2 = instantiate_oriented_simplex<0>();
    auto vertex_3 = instantiate_oriented_simplex<0>();
    auto vertex_4 = instantiate_oriented_simplex<0>();

    // assert that all vertices are referenced once
    EXPECT_EQ(vertex_0.references(), 1);
    EXPECT_EQ(vertex_1.references(), 1);
    EXPECT_EQ(vertex_2.references(), 1);
    EXPECT_EQ(vertex_3.references(), 1);
    EXPECT_EQ(vertex_4.references(), 1);

    // instantiate segments
    auto segment_a = instantiate_simplex<1>(vertex_0, vertex_1);
    auto segment_b = instantiate_simplex<1>(vertex_1, vertex_3);
    auto segment_c = instantiate_simplex<1>(vertex_3, vertex_0);
    auto segment_d = instantiate_simplex<1>(vertex_1, vertex_2);
    auto segment_e = instantiate_simplex<1>(vertex_2, vertex_3);
    auto segment_f = instantiate_simplex<1>(vertex_4, vertex_3);
    auto segment_g = instantiate_simplex<1>(vertex_2, vertex_4);
    auto segment_h = instantiate_simplex<1>(vertex_4, vertex_0);

    // assert that all vertices are referenced once
    EXPECT_EQ(segment_a.references(), 1);
    EXPECT_EQ(segment_b.references(), 1);
    EXPECT_EQ(segment_c.references(), 1);
    EXPECT_EQ(segment_d.references(), 1);
    EXPECT_EQ(segment_e.references(), 1);
    EXPECT_EQ(segment_f.references(), 1);
    EXPECT_EQ(segment_g.references(), 1);
    EXPECT_EQ(segment_h.references(), 1);

    // assert that all vertices on the boundary are referenced four times (once plus once per each
    // of the 3 segments stemming from a corner)
    EXPECT_EQ(vertex_0.references(), 4);
    EXPECT_EQ(vertex_1.references(), 4);
    EXPECT_EQ(vertex_2.references(), 4);
    EXPECT_EQ(vertex_4.references(), 4);

    // assert that the vertex in the center is referenced five times (once plus once per each
    // of the 4 segments stemming from the center)
    EXPECT_EQ(vertex_3.references(), 5);

    // instantiate oriented segments and (unoriented) triangles
    auto oriented_segment_0 = instantiate_oriented_simplex<1>(segment_a, true);
    auto oriented_segment_1 = instantiate_oriented_simplex<1>(segment_b, true);
    auto oriented_segment_2 = instantiate_oriented_simplex<1>(segment_c, true);
    auto triangle_a =
        instantiate_simplex<2>(oriented_segment_0, oriented_segment_1, oriented_segment_2);

    auto oriented_segment_3 = instantiate_oriented_simplex<1>(segment_d, true);
    auto oriented_segment_4 = instantiate_oriented_simplex<1>(segment_e, true);
    auto oriented_segment_5 = instantiate_oriented_simplex<1>(segment_b, false);
    auto triangle_b =
        instantiate_simplex<2>(oriented_segment_3, oriented_segment_4, oriented_segment_5);

    auto oriented_segment_6 = instantiate_oriented_simplex<1>(segment_g, true);
    auto oriented_segment_7 = instantiate_oriented_simplex<1>(segment_f, true);
    auto oriented_segment_8 = instantiate_oriented_simplex<1>(segment_e, false);
    auto triangle_c =
        instantiate_simplex<2>(oriented_segment_6, oriented_segment_7, oriented_segment_8);

    auto oriented_segment_9 = instantiate_oriented_simplex<1>(segment_h, true);
    auto oriented_segment_10 = instantiate_oriented_simplex<1>(segment_c, false);
    auto oriented_segment_11 = instantiate_oriented_simplex<1>(segment_f, false);
    auto triangle_d =
        instantiate_simplex<2>(oriented_segment_9, oriented_segment_10, oriented_segment_11);

    // assert that all triangles are referenced once
    EXPECT_EQ(triangle_a.references(), 1);
    EXPECT_EQ(triangle_b.references(), 1);
    EXPECT_EQ(triangle_c.references(), 1);
    EXPECT_EQ(triangle_d.references(), 1);

    // assert that all segments on the boundary are referenced 2 times (once plus once for the
    // corresponding triangle on the boundary)
    EXPECT_EQ(segment_a.references(), 2);
    EXPECT_EQ(segment_d.references(), 2);
    EXPECT_EQ(segment_g.references(), 2);
    EXPECT_EQ(segment_h.references(), 2);

    // assert that all internal segments are referenced 3 times (once plus once per pair of
    // triangles sharing the segment)
    EXPECT_EQ(segment_b.references(), 3);
    EXPECT_EQ(segment_c.references(), 3);
    EXPECT_EQ(segment_e.references(), 3);
    EXPECT_EQ(segment_f.references(), 3);

    // instantiate oriented triangles
    auto triangle_0 = instantiate_oriented_simplex<2>(triangle_a, true);
    auto triangle_1 = instantiate_oriented_simplex<2>(triangle_b, true);
    auto triangle_2 = instantiate_oriented_simplex<2>(triangle_c, true);
    auto triangle_3 = instantiate_oriented_simplex<2>(triangle_d, true);

    // assert that all triangles are referenced once
    EXPECT_EQ(triangle_a.references(), 2);
    EXPECT_EQ(triangle_b.references(), 2);
    EXPECT_EQ(triangle_c.references(), 2);
    EXPECT_EQ(triangle_d.references(), 2);

    // // erase one triangle (by hand)
    // triangle_0->erase();
    // triangle_a->erase();
    // oriented_segment_0->erase();
    // segment_a->erase();
    // oriented_segment_1->erase();
    // // segment_b->erase(); // do not erase: other triangles use it
    // oriented_segment_2->erase();
    // // segment_c->erase(); // do not erase: other triangles use it

    // // erase one triangle (non recursive)
    // {
    //     // grab a copy of the footprint
    //     auto footprint = triangle_0->footprint();

    //     // grab a copy of the composition
    //     auto composition = triangle_0->composition();

    //     triangle_0->erase();
    //     if (footprint.references() == 2) {
    //         footprint->erase();
    //     }

    //     // loop on the subsimplices
    //     for (auto & simplex : composition) {
    //         // std::cout << simplex.references() << std::endl;
    //         if (simplex.references() == 2) {
    //             // grab a copy of the footprint
    //             auto footprint_2 = simplex->footprint();
    //             simplex->erase();
    //             if (footprint_2.references() == 2) {
    //                 footprint_2->erase();
    //                 // fetch vertices...
    //             }
    //         }
    //     }
    // }

    // erase one triangle (recursive)
    erase(triangle_0);

    // assert that the footprint of {triangle_0} is now unused
    EXPECT_EQ(triangle_a.references(), 1);

    // assert that each oriented segment of {triangle_0} is now unused
    EXPECT_EQ(oriented_segment_0.references(), 1);
    EXPECT_EQ(oriented_segment_1.references(), 1);
    EXPECT_EQ(oriented_segment_2.references(), 1);

    // assert that the boundary segment of {triangle_0} is now unused
    EXPECT_EQ(segment_a.references(), 1);
    // assert that the other segments of {triangle_0} are now used only once
    EXPECT_EQ(segment_b.references(), 2);
    EXPECT_EQ(segment_c.references(), 2);

    // assert that vertices on the boundary are now used one time less, whereas the vertex in the
    // center is still used by the internal unoriented segments, which have not been erased
    EXPECT_EQ(vertex_0.references(), 3);
    EXPECT_EQ(vertex_1.references(), 3);
    EXPECT_EQ(vertex_3.references(), 5);

    // erase all other triangles
    erase(triangle_1);
    erase(triangle_2);
    erase(triangle_3);

    // assert everything is now unused
    EXPECT_EQ(triangle_a.references(), 1);
    EXPECT_EQ(triangle_b.references(), 1);
    EXPECT_EQ(triangle_c.references(), 1);
    EXPECT_EQ(triangle_d.references(), 1);
    EXPECT_EQ(oriented_segment_0.references(), 1);
    EXPECT_EQ(oriented_segment_1.references(), 1);
    EXPECT_EQ(oriented_segment_2.references(), 1);
    EXPECT_EQ(oriented_segment_3.references(), 1);
    EXPECT_EQ(oriented_segment_4.references(), 1);
    EXPECT_EQ(oriented_segment_5.references(), 1);
    EXPECT_EQ(oriented_segment_6.references(), 1);
    EXPECT_EQ(oriented_segment_7.references(), 1);
    EXPECT_EQ(oriented_segment_8.references(), 1);
    EXPECT_EQ(oriented_segment_9.references(), 1);
    EXPECT_EQ(oriented_segment_10.references(), 1);
    EXPECT_EQ(oriented_segment_11.references(), 1);
    EXPECT_EQ(segment_a.references(), 1);
    EXPECT_EQ(segment_b.references(), 1);
    EXPECT_EQ(segment_c.references(), 1);
    EXPECT_EQ(segment_d.references(), 1);
    EXPECT_EQ(segment_e.references(), 1);
    EXPECT_EQ(segment_f.references(), 1);
    EXPECT_EQ(segment_g.references(), 1);
    EXPECT_EQ(segment_h.references(), 1);
    EXPECT_EQ(vertex_0.references(), 1);
    EXPECT_EQ(vertex_1.references(), 1);
    EXPECT_EQ(vertex_2.references(), 1);
    EXPECT_EQ(vertex_3.references(), 1);
    EXPECT_EQ(vertex_4.references(), 1);
}
