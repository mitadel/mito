#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(EraseElement, TestEraseElementMesh)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // ask the geometry for new nodes (this instantiates a new vertex and attaches it to the point)
    auto & node0 = geometry.node({ 0.0, 0.0 });
    // or equivalently
    // auto point0 = point_cloud.point({ 0.0, 0.0 });
    // auto & node0 = geometry.node(topology.vertex(), point0);
    auto & node1 = geometry.node({ 1.0, 0.0 });
    auto & node2 = geometry.node({ 1.0, 1.0 });
    auto & node3 = geometry.node({ 0.5, 0.5 });
    auto & node4 = geometry.node({ 0.0, 1.0 });

    // ask the topology for segments connecting the vertices of the nodes above
    auto & segment0 = topology.segment({ node0.vertex(), node1.vertex() });
    auto & segment1 = topology.segment({ node1.vertex(), node3.vertex() });
    auto & segment2 = topology.segment({ node3.vertex(), node0.vertex() });
    auto & cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto & segment3 = topology.segment({ node1.vertex(), node2.vertex() });
    auto & segment4 = topology.segment({ node2.vertex(), node3.vertex() });
    auto & segment5 = topology.segment({ node3.vertex(), node1.vertex() });
    auto & cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto & segment6 = topology.segment({ node2.vertex(), node4.vertex() });
    auto & segment7 = topology.segment({ node4.vertex(), node3.vertex() });
    auto & segment8 = topology.segment({ node3.vertex(), node2.vertex() });
    auto & cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto & segment9 = topology.segment({ node4.vertex(), node0.vertex() });
    auto & segment10 = topology.segment({ node0.vertex(), node3.vertex() });
    auto & segment11 = topology.segment({ node3.vertex(), node4.vertex() });
    auto & cell3 = topology.triangle({ segment9, segment10, segment11 });

    // an empty mesh of triangles in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);
    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);

    // show me the cells
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>())
    // {
    //     std::cout << simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    mesh.erase(cell0);

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>()) {
    //     std::cout << simplex << std::endl;
    // }

    // assert the mesh has now 3 cells
    EXPECT_EQ(mesh.nCells(), 3);

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mesh.boundary().nCells(), 5);

    // show me the boundary cells
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mito::mesh::boundary<1>(mesh, topology)) {
    //     std::cout << simplex << std::endl;
    // }

    mesh.erase(cell1);

    // assert the mesh has now 2 cells
    EXPECT_EQ(mesh.nCells(), 2);

    // assert the boundary is now made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);
}

#if 0
template <int D>
auto
instantiate_simplex(mito::topology::simplex_composition_t<D> composition)
    -> mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>>
requires(D > 0)
{
    mito::topology::unoriented_simplex_t<D> * resource =
        new mito::topology::unoriented_simplex_t<D>(composition);
    return mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>>(resource, nullptr);
}

template <int D>
auto
instantiate_oriented_simplex(
    const mito::utilities::shared_ptr<mito::topology::unoriented_simplex_t<D>> & simplex,
    bool orientation) -> mito::utilities::shared_ptr<mito::topology::oriented_simplex_t<D>>
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
    simplex->_erase();

    // if this simplex is the last one using the footprint
    if (footprint.references() == 2) {
        // reset it
        footprint->_erase();
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

TEST(EraseElement, TestEraseElementBase)
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
    auto segment_a = instantiate_simplex<1>({ vertex_0, vertex_1 });
    auto segment_b = instantiate_simplex<1>({ vertex_1, vertex_3 });
    auto segment_c = instantiate_simplex<1>({ vertex_3, vertex_0 });
    auto segment_d = instantiate_simplex<1>({ vertex_1, vertex_2 });
    auto segment_e = instantiate_simplex<1>({ vertex_2, vertex_3 });
    auto segment_f = instantiate_simplex<1>({ vertex_4, vertex_3 });
    auto segment_g = instantiate_simplex<1>({ vertex_2, vertex_4 });
    auto segment_h = instantiate_simplex<1>({ vertex_4, vertex_0 });

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
        instantiate_simplex<2>({ oriented_segment_0, oriented_segment_1, oriented_segment_2 });

    auto oriented_segment_3 = instantiate_oriented_simplex<1>(segment_d, true);
    auto oriented_segment_4 = instantiate_oriented_simplex<1>(segment_e, true);
    auto oriented_segment_5 = instantiate_oriented_simplex<1>(segment_b, false);
    auto triangle_b =
        instantiate_simplex<2>({ oriented_segment_3, oriented_segment_4, oriented_segment_5 });

    auto oriented_segment_6 = instantiate_oriented_simplex<1>(segment_g, true);
    auto oriented_segment_7 = instantiate_oriented_simplex<1>(segment_f, true);
    auto oriented_segment_8 = instantiate_oriented_simplex<1>(segment_e, false);
    auto triangle_c =
        instantiate_simplex<2>({ oriented_segment_6, oriented_segment_7, oriented_segment_8 });

    auto oriented_segment_9 = instantiate_oriented_simplex<1>(segment_h, true);
    auto oriented_segment_10 = instantiate_oriented_simplex<1>(segment_c, false);
    auto oriented_segment_11 = instantiate_oriented_simplex<1>(segment_f, false);
    auto triangle_d =
        instantiate_simplex<2>({ oriented_segment_9, oriented_segment_10, oriented_segment_11 });

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
    // triangle_0->_erase();
    // triangle_a->_erase();
    // oriented_segment_0->_erase();
    // segment_a->_erase();
    // oriented_segment_1->_erase();
    // // segment_b->_erase(); // do not erase: other triangles use it
    // oriented_segment_2->_erase();
    // // segment_c->_erase(); // do not erase: other triangles use it

    // // erase one triangle (non recursive)
    // {
    //     // grab a copy of the footprint
    //     auto footprint = triangle_0->footprint();

    //     // grab a copy of the composition
    //     auto composition = triangle_0->composition();

    //     triangle_0->_erase();
    //     if (footprint.references() == 2) {
    //         footprint->_erase();
    //     }

    //     // loop on the subsimplices
    //     for (auto & simplex : composition) {
    //         // std::cout << simplex.references() << std::endl;
    //         if (simplex.references() == 2) {
    //             // grab a copy of the footprint
    //             auto footprint_2 = simplex->footprint();
    //             simplex->_erase();
    //             if (footprint_2.references() == 2) {
    //                 footprint_2->_erase();
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
#endif

TEST(EraseElement, TestEraseElementTopology)
{
    /**
     * Mesh with four cells:
    (0,1)           (1,1)
      4               2
      +---------------+
      | .           . |
      |   .       .   |
      |     .   .     |
      |       . 3     |
      |     .   .     |
      |   .       .   |
      | .           . |
      +---------------+
      0               1
    (0,0)           (1,0)
    */

    // an empty topology
    auto & topology = mito::topology::topology();

    const auto & vertex_0 = topology.vertex();
    const auto & vertex_1 = topology.vertex();
    const auto & vertex_2 = topology.vertex();
    const auto & vertex_3 = topology.vertex();
    const auto & vertex_4 = topology.vertex();

    const auto & segment_0 = topology.segment({ vertex_0, vertex_1 });
    const auto & segment_1 = topology.segment({ vertex_1, vertex_3 });
    const auto & segment_2 = topology.segment({ vertex_3, vertex_0 });
    const auto & cell_0 = topology.triangle({ segment_0, segment_1, segment_2 });

    const auto & segment_3 = topology.segment({ vertex_1, vertex_2 });
    const auto & segment_4 = topology.segment({ vertex_2, vertex_3 });
    const auto & segment_5 = topology.segment({ vertex_3, vertex_1 });
    const auto & cell_1 = topology.triangle({ segment_3, segment_4, segment_5 });

    const auto & segment_6 = topology.segment({ vertex_2, vertex_4 });
    const auto & segment_7 = topology.segment({ vertex_4, vertex_3 });
    const auto & segment_8 = topology.segment({ vertex_3, vertex_2 });
    [[maybe_unused]] const auto & cell_2 = topology.triangle({ segment_6, segment_7, segment_8 });

    const auto & segment_9 = topology.segment({ vertex_4, vertex_0 });
    const auto & segment_10 = topology.segment({ vertex_0, vertex_3 });
    const auto & segment_11 = topology.segment({ vertex_3, vertex_4 });
    [[maybe_unused]] const auto & cell_3 = topology.triangle({ segment_9, segment_10, segment_11 });

    // assert that {segment_5} (which is the flipped counterpart of {segment_1}) has a flipped
    // counterpart in the topology (namely, {segment_1})
    EXPECT_EQ(topology.exists_flipped(segment_5), true);
    // idem for {segment_10} (which is the flipped counterpart of newly deleted {segment_2})
    EXPECT_EQ(topology.exists_flipped(segment_10), true);
    // idem for {segment_8} (which is the flipped counterpart of newly deleted {segment_4})
    EXPECT_EQ(topology.exists_flipped(segment_8), true);

    // erase the cell with edges {segment_0, segment_1, segment_2}
    topology.erase(cell_0);

    // assert that {segment_5} (which is the flipped counterpart of newly deleted {segment_1}) does
    // not have a flipped counterpart in the topology any longer
    EXPECT_EQ(topology.exists_flipped(segment_5), false);
    // idem for {segment_10} (which is the flipped counterpart of newly deleted {segment_2})
    EXPECT_EQ(topology.exists_flipped(segment_10), false);

    // erase the cell with edges {segment_3, segment_4, segment_5}, which used to share the
    // footprint of {segment_5} with {segment_1} of {cell_0}
    topology.erase(cell_1);
    // assert that {segment_8} (which is the flipped counterpart of newly deleted {segment_4}) does
    // not have a flipped counterpart in the topology any longer
    EXPECT_EQ(topology.exists_flipped(segment_8), false);
}