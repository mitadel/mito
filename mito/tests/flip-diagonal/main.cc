#include "../../base.h"
#include "../../mesh.h"
#include <set>

const mito::mesh::simplex_t<1> *
findSharedSimplex(
    mito::mesh::oriented_simplex_ptr<2> & element_0,
    mito::mesh::oriented_simplex_ptr<2> & element_1)
{
    // loop on lower-dimensional simplices to find the edge shared by the two elements
    for (const auto & subsimplex_0 : element_0->simplices()) {
        for (const auto & subsimplex_1 : element_1->simplices()) {
            // if you found it
            if (&subsimplex_0->simplex() == &subsimplex_1->simplex()) {
                // report
                std::cout << "Found it!" << std::endl;
                // return
                return &subsimplex_0->simplex();
            }
        }
    }

    // all done
    return nullptr;
}

mito::mesh::vertex_vector_t
oppositeVertices(
    mito::mesh::oriented_simplex_ptr<2> & element_0,
    mito::mesh::oriented_simplex_ptr<2> & element_1,
    const mito::mesh::simplex_t<1> * shared_simplex)
{
    // need a regular set (not an unordered one) because set_difference works with ordered sets
    using vertex_set_t = std::set<std::shared_ptr<const mito::mesh::vertex_t>>;

    vertex_set_t vertices;
    element_0->vertices(vertices);
    element_1->vertices(vertices);

    vertex_set_t shared_simplex_vertices;
    shared_simplex->vertices(shared_simplex_vertices);

    vertex_set_t opposite_vertices;
    std::set_difference(
        vertices.begin(), vertices.end(), shared_simplex_vertices.begin(),
        shared_simplex_vertices.end(), std::inserter(opposite_vertices, opposite_vertices.end()));

    mito::mesh::vertex_vector_t opposite_vertices_vector(
        opposite_vertices.begin(), opposite_vertices.end());
    assert(opposite_vertices.size() == 2);

    return opposite_vertices_vector;
}

bool
headTailConnects(
    mito::mesh::oriented_simplex_ptr<1> simplex_1, mito::mesh::oriented_simplex_ptr<1> simplex_2)
{
    if (mito::mesh::head(simplex_1) == mito::mesh::tail(simplex_2)) {
        return true;
    }

    return false;
}

int
flipDiagonal(
    mito::mesh::oriented_simplex_ptr<2> & element_0,
    mito::mesh::oriented_simplex_ptr<2> & element_1)
{
    // get the shared simplex between the two elements
    const mito::mesh::simplex_t<1> * shared_simplex = findSharedSimplex(element_0, element_1);

    // assert you could find it
    assert(shared_simplex != nullptr);

    // show me
    // std::cout << "shared simplex: " << *shared_simplex << std::endl;

    auto opposite_vertices = oppositeVertices(element_0, element_1, shared_simplex);

    auto diagonal_segment = mito::mesh::segment({ opposite_vertices[0], opposite_vertices[1] });
    auto opposite_diagonal_segment =
        mito::mesh::segment({ opposite_vertices[1], opposite_vertices[0] });

    std::set<mito::mesh::oriented_simplex_ptr<1>> boundary_simplices;
    // get boundary simplices of element_0 (all except diagonal)
    for (const auto & subsimplex : element_0->simplices()) {
        // if it is not the shared simplex
        if (&subsimplex->simplex() != shared_simplex) {
            boundary_simplices.insert(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 2);
    // get boundary simplices of element_1 (all except diagonal)
    for (const auto & subsimplex : element_1->simplices()) {
        // if it is not the shared simplex
        if (&subsimplex->simplex() != shared_simplex) {
            boundary_simplices.insert(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 4);

    mito::mesh::simplex_composition_t<2> new_element_composition_0;
    new_element_composition_0[0] = diagonal_segment;

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_element_composition_0[0], subsimplex)) {
            new_element_composition_0[1] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 3);

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_element_composition_0[1], subsimplex)) {
            new_element_composition_0[2] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 2);

    assert(headTailConnects(new_element_composition_0[2], new_element_composition_0[0]));

    mito::mesh::simplex_composition_t<2> new_element_composition_1;
    new_element_composition_1[0] = opposite_diagonal_segment;

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_element_composition_1[0], subsimplex)) {
            new_element_composition_1[1] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 1);

    for (const auto & subsimplex : boundary_simplices) {
        if (headTailConnects(new_element_composition_1[1], subsimplex)) {
            new_element_composition_1[2] = subsimplex;
            boundary_simplices.erase(subsimplex);
        }
    }
    assert(boundary_simplices.size() == 0);

    assert(headTailConnects(new_element_composition_1[2], new_element_composition_1[0]));

    // TOFIX: how to delete the old simplices?

    // delete old simplices

    // build new simplices
    auto new_element_0 = mito::mesh::triangle(new_element_composition_0);
    auto new_element_1 = mito::mesh::triangle(new_element_composition_1);

    // TOFIX: how to return the new elements?

    // all done
    return 0;
}

int
main()
{
    // TOFIX: vertices factories discussed with Michael
    // auto points = mito::mesh::point_cloud<2>();
    // auto vertices = mito::mesh::vertex_cloud();
    // auto vertex0 = vertices.newVertex();

    // build vertices
    auto vertex0 = mito::mesh::vertex();
    auto vertex1 = mito::mesh::vertex();
    auto vertex2 = mito::mesh::vertex();
    auto vertex3 = mito::mesh::vertex();

    // build segments
    auto segment_a = mito::mesh::segment({ vertex0, vertex1 });
    auto segment_b = mito::mesh::segment({ vertex1, vertex2 });
    auto segment_c = mito::mesh::segment({ vertex2, vertex3 });
    auto segment_d = mito::mesh::segment({ vertex3, vertex0 });
    auto segment_e = mito::mesh::segment({ vertex0, vertex2 });
    auto segment_e_flip = mito::mesh::flip(segment_e);

    // build triangles
    auto element_0 = mito::mesh::triangle({ segment_a, segment_b, segment_e_flip });
    auto element_1 = mito::mesh::triangle({ segment_e, segment_c, segment_d });

    // flip the common edge of the two triangles
    flipDiagonal(element_0, element_1);

    // all done
    return 0;
}
