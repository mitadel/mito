#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <set>

const auto &
findSharedSimplex(
    const mito::topology::simplex_t<2> & simplex0, const mito::topology::simplex_t<2> & simplex1)
{
    // loop on lower-dimensional simplices to find the edge shared by the two simplices
    for (const auto & subsimplex0 : simplex0->composition()) {
        for (const auto & subsimplex1 : simplex1->composition()) {
            // if you found it
            if (subsimplex0->footprint_id() == subsimplex1->footprint_id()) {
                // report
                std::cout << "Found it!" << std::endl;
                // return
                return subsimplex0->footprint();
            }
        }
    }

    // TOFIX: throw error with journal, but control reaches end of non-void function
    // pyre::journal::error_t error("error");
    // error << "Found no shared simplices" << pyre::journal::endl;
    // something went wrong
    throw std::logic_error("Found no shared simplices.");
}

mito::topology::vertex_vector_t
oppositeVertices(
    const mito::topology::simplex_t<2> & simplex0, const mito::topology::simplex_t<2> & simplex1,
    const auto & shared_simplex)
{
    // need a regular set (not an unordered one) because set_difference works with ordered sets
    using vertex_set_t = std::set<mito::topology::simplex_t<0>>;

    vertex_set_t vertices;
    simplex0->vertices(vertices);
    simplex1->vertices(vertices);

    vertex_set_t shared_simplex_vertices;
    shared_simplex->vertices(shared_simplex_vertices);

    vertex_set_t opposite_vertices;
    std::set_difference(
        vertices.begin(), vertices.end(), shared_simplex_vertices.begin(),
        shared_simplex_vertices.end(), std::inserter(opposite_vertices, opposite_vertices.end()));

    mito::topology::vertex_vector_t opposite_vertices_vector(
        opposite_vertices.begin(), opposite_vertices.end());
    EXPECT_EQ(opposite_vertices.size(), 2);

    return opposite_vertices_vector;
}

bool
headTailConnects(
    const mito::topology::simplex_t<1> & simplex_1, const mito::topology::simplex_t<1> & simplex_2)
{
    if (mito::topology::head(simplex_1) == mito::topology::tail(simplex_2)) {
        return true;
    }

    return false;
}

int
flipDiagonal(
    const mito::topology::simplex_t<2> & simplex0, const mito::topology::simplex_t<2> & simplex1,
    mito::topology::topology_t & topology)
{
    // get the shared simplex between the two simplices
    const auto & shared_simplex = findSharedSimplex(simplex0, simplex1);

    // assert you could find it
    EXPECT_TRUE(shared_simplex.handle() != nullptr);

    // show me
    // std::cout << "shared simplex: " << *shared_simplex << std::endl;

    auto opposite_vertices = oppositeVertices(simplex0, simplex1, shared_simplex);

    auto diagonal_segment = topology.segment({ opposite_vertices[0], opposite_vertices[1] });
    auto opposite_diagonal_segment =
        topology.segment({ opposite_vertices[1], opposite_vertices[0] });

    std::set<mito::topology::simplex_t<1>> boundary_simplices;
    // get boundary simplices of simplex0 (all except diagonal)
    for (const auto & subsimplex : simplex0->composition()) {
        // if it is not the shared simplex
        if (subsimplex->footprint_id() != shared_simplex->id()) {
            boundary_simplices.insert(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 2);
    // get boundary simplices of simplex1 (all except diagonal)
    for (const auto & subsimplex : simplex1->composition()) {
        // if it is not the shared simplex
        if (subsimplex->footprint_id() != shared_simplex->id()) {
            boundary_simplices.insert(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 4);

    mito::topology::simplex_composition_t<2> new_simplex_composition_0;
    new_simplex_composition_0[0] = diagonal_segment;

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_simplex_composition_0[0], subsimplex)) {
            new_simplex_composition_0[1] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 3);

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_simplex_composition_0[1], subsimplex)) {
            new_simplex_composition_0[2] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 2);

    EXPECT_TRUE(headTailConnects(new_simplex_composition_0[2], new_simplex_composition_0[0]));

    mito::topology::simplex_composition_t<2> new_simplex_composition_1;
    new_simplex_composition_1[0] = opposite_diagonal_segment;

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_simplex_composition_1[0], subsimplex)) {
            new_simplex_composition_1[1] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 1);

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_simplex_composition_1[1], subsimplex)) {
            new_simplex_composition_1[2] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    EXPECT_EQ(boundary_simplices.size(), 0);

    EXPECT_TRUE(headTailConnects(new_simplex_composition_1[2], new_simplex_composition_1[0]));

    // TOFIX: how to delete the old simplices?

    // delete old simplices

    // build new simplices
    auto new_simplex0 = topology.triangle(new_simplex_composition_0);
    auto new_simplex1 = topology.triangle(new_simplex_composition_1);

    // TOFIX: how to return the new simplices?

    // all done
    return 0;
}

TEST(FlipDiagonal, TestFlipDiagonal)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build vertices
    auto & vertex0 = topology.vertex();
    auto & vertex1 = topology.vertex();
    auto & vertex2 = topology.vertex();
    auto & vertex3 = topology.vertex();

    // build segments
    auto & segment_a = topology.segment({ vertex0, vertex1 });
    auto & segment_b = topology.segment({ vertex1, vertex2 });
    auto & segment_c = topology.segment({ vertex2, vertex3 });
    auto & segment_d = topology.segment({ vertex3, vertex0 });
    auto & segment_e = topology.segment({ vertex0, vertex2 });
    auto & segment_e_flip = topology.flip(segment_e);

    // build triangles
    auto & simplex0 = topology.triangle({ segment_a, segment_b, segment_e_flip });
    auto & simplex1 = topology.triangle({ segment_e, segment_c, segment_d });

    // flip the common edge of the two triangles
    flipDiagonal(simplex0, simplex1, topology);
}
