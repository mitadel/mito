// code guard
#if !defined(mito_mesh_flipdiagonal_h)
#define mito_mesh_flipdiagonal_h


namespace mito::mesh {


    auto findSharedSimplex(
        const topology::simplex_t<2> & simplex0, const topology::simplex_t<2> & simplex1)
        -> const topology::unoriented_simplex_t<1> &
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


    auto oppositeVertices(
        const topology::simplex_t<2> & simplex0, const topology::simplex_t<2> & simplex1,
        const auto & shared_simplex) -> topology::vertex_vector_t
    {
        // need a regular set (not an unordered one) because set_difference works with ordered sets
        using vertex_set_t = std::set<topology::vertex_t>;

        vertex_set_t vertices;
        simplex0->vertices(vertices);
        simplex1->vertices(vertices);

        vertex_set_t shared_simplex_vertices;
        shared_simplex->vertices(shared_simplex_vertices);

        vertex_set_t opposite_vertices;
        std::set_difference(
            vertices.begin(), vertices.end(), shared_simplex_vertices.begin(),
            shared_simplex_vertices.end(),
            std::inserter(opposite_vertices, opposite_vertices.end()));

        topology::vertex_vector_t opposite_vertices_vector(
            opposite_vertices.begin(), opposite_vertices.end());
        assert(opposite_vertices.size() == 2);

        return opposite_vertices_vector;
    }


    template <int D, int N>
    auto flipDiagonal(
        mesh_t<topology::simplex_t<N>, D> & mesh, const topology::simplex_t<2> & simplex0,
        const topology::simplex_t<2> & simplex1) -> void
    {
        // get the shared simplex between the two simplices
        const auto & shared_simplex = findSharedSimplex(simplex0, simplex1);

        // assert you could find it
        assert(!shared_simplex.is_nullptr());

        // show me
        // std::cout << "shared simplex: " << *shared_simplex << std::endl;
        auto opposite_vertices = oppositeVertices(simplex0, simplex1, shared_simplex);

        auto & topology = topology::topology();

        auto diagonal_segment = topology.segment({ opposite_vertices[0], opposite_vertices[1] });
        auto opposite_diagonal_segment =
            topology.segment({ opposite_vertices[1], opposite_vertices[0] });

        std::set<topology::simplex_t<1>> boundary_simplices;
        // get boundary simplices of simplex0 (all except diagonal)
        for (const auto & subsimplex : simplex0->composition()) {
            // if it is not the shared simplex
            if (subsimplex->footprint_id() != shared_simplex->id()) {
                boundary_simplices.insert(subsimplex);
            }
        }
        assert(boundary_simplices.size() == 2);
        // get boundary simplices of simplex1 (all except diagonal)
        for (const auto & subsimplex : simplex1->composition()) {
            // if it is not the shared simplex
            if (subsimplex->footprint_id() != shared_simplex->id()) {
                boundary_simplices.insert(subsimplex);
            }
        }
        assert(boundary_simplices.size() == 4);

        topology::simplex_composition_t<2> new_simplex_composition_0;
        new_simplex_composition_0[0] = diagonal_segment;

        topology::simplex_t<1> subsimplex_to_erase = *boundary_simplices.begin();
        for (const auto & subsimplex : boundary_simplices) {
            if (headTailConnected(new_simplex_composition_0[0], subsimplex)) {
                new_simplex_composition_0[1] = subsimplex;
                subsimplex_to_erase = subsimplex;
                break;
            }
        }
        boundary_simplices.erase(subsimplex_to_erase);
        topology.erase(subsimplex_to_erase);
        assert(boundary_simplices.size() == 3);

        for (const auto & subsimplex : boundary_simplices) {
            if (headTailConnected(new_simplex_composition_0[1], subsimplex)) {
                new_simplex_composition_0[2] = subsimplex;
                subsimplex_to_erase = subsimplex;
                break;
            }
        }
        boundary_simplices.erase(subsimplex_to_erase);
        topology.erase(subsimplex_to_erase);
        assert(boundary_simplices.size() == 2);

        assert(headTailConnected(new_simplex_composition_0[2], new_simplex_composition_0[0]));

        topology::simplex_composition_t<2> new_simplex_composition_1;
        new_simplex_composition_1[0] = opposite_diagonal_segment;

        for (const auto & subsimplex : boundary_simplices) {
            if (headTailConnected(new_simplex_composition_1[0], subsimplex)) {
                new_simplex_composition_1[1] = subsimplex;
                subsimplex_to_erase = subsimplex;
                break;
            }
        }
        boundary_simplices.erase(subsimplex_to_erase);
        topology.erase(subsimplex_to_erase);
        assert(boundary_simplices.size() == 1);

        for (const auto & subsimplex : boundary_simplices) {
            if (headTailConnected(new_simplex_composition_1[1], subsimplex)) {
                new_simplex_composition_1[2] = subsimplex;
                subsimplex_to_erase = subsimplex;
                break;
            }
        }
        boundary_simplices.erase(subsimplex_to_erase);
        topology.erase(subsimplex_to_erase);
        assert(boundary_simplices.size() == 0);

        assert(headTailConnected(new_simplex_composition_1[2], new_simplex_composition_1[0]));

        // build new simplices
        auto & new_simplex0 = topology.triangle(new_simplex_composition_0);
        auto & new_simplex1 = topology.triangle(new_simplex_composition_1);

        mesh.insert(new_simplex0);
        mesh.erase(simplex0);
        topology.erase(simplex0);

        mesh.insert(new_simplex1);
        mesh.erase(simplex1);
        topology.erase(simplex1);

        // all done
        return;
    }
}


#endif

// end of file
