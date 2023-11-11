// code guard
#if !defined(mito_topology_utilities_h)
#define mito_topology_utilities_h


namespace mito::topology {
    // order of simplex
    template <class cellT>
    constexpr auto order() -> int
    {
        return cellT::resource_type::order;
    }

    // number of vertices of simplex
    template <class cellT>
    constexpr auto n_vertices() -> int
    {
        return cellT::resource_type::n_vertices;
    }

    // cell family of simplex
    template <class cellT, int I>
    using cell_family = typename cellT::resource_type::template cell_family_type<I>;

    // overload operator<< for oriented simplices
    template <int N>
    std::ostream & operator<<(std::ostream & os, const simplex_t<N> & s)
    {
        // print orientation
        os << "orientation: " << s->orientation() << std::endl;
        // print footprint
        os << "footprint: " << s->footprint() << std::endl;
        // all done
        return os;
    }

    // overload operator<< for simplices
    template <int N>
    std::ostream & operator<<(std::ostream & os, const unoriented_simplex_t<N> & s)
    {
        os << s.id() << " composed of:" << std::endl;
        for (const auto & simplex : s->composition()) {
            std::cout << "\t" << simplex << std::endl;
        }
        return os;
    }

    // overload operator<< for oriented simplices
    template <>
    std::ostream & operator<<(std::ostream & os, const unoriented_simplex_t<0> & s)
    {
        os << "vertex: " << s.id() << std::endl;
        return os;
    }

    auto tail(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        return oriented_simplex->composition()[0]->footprint();
    }

    auto head(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        return oriented_simplex->composition()[1]->footprint();
    }

    bool headTailConnected(const simplex_t<1> & simplex_1, const simplex_t<1> & simplex_2)
    {
        if (head(simplex_1) == tail(simplex_2)) {
            return true;
        }

        return false;
    }

    // checks whether a given simplex composition is valid, i.e. if all simplices in {composition}
    // have consistent orientations with each other
    // NOTE: the result of this function should not depend on the order of the simplices in
    //          {composition}
    // TODO: add a test to check this
    template <int N>
    auto isValid(const simplex_composition_t<N> & composition) -> bool
    requires(N > 1)
    {
        // id type of unoriented simplex
        using cell_id_type = utilities::index_t<unoriented_simplex_t<N>>;
        // map from a simplex id to an integer
        using orientation_map_type = std::unordered_map<cell_id_type, int>;
        // map to store the net orientation (i.e. positive-orientation occurrences minus
        // negative-orientation occurrences) of a given unoriented simplex
        // this map is used to determine whether the simplices in composition have consistent
        // orientation with each other (i.e. if its (N-2)-subsimplices appear two
        // times with two opposite orientations
        orientation_map_type orientation_map;

        // loop on all the (N-1)-simplices in {composition}
        for (const auto & simplex : composition) {
            // loop on all the (N-2)-simplices in {composition}
            for (const auto & subsimplex : simplex->composition()) {
                // populate the orientation map with the {subsimplex} id and its 'net' orientation
                orientation_map[subsimplex->footprint().id()] +=
                    subsimplex->orientation() * simplex->orientation();
            }
        }

        // assert that the {orientation_map} has as many entries as the number of (unoriented) edges
        // in an N-simplex
        assert(std::size(orientation_map) == ((N + 1) * N) / 2);

        // check that the net orientation of each (N-2)-subsimplex is 0
        // (i.e. the subsimplex appears twice, with each opposite orientations)
        for (const auto & item : orientation_map) {
            // if any of the simplices in {orientation_map} has not zero net orientation within the
            // {composition}...
            if (item.second != 0) {
                // {composition} is not a valid simplex composition
                return false;
            }
        }

        // otherwise, {composition} is a valid simplex composition
        return true;
    }

    template <int N>
    auto isValid(const simplex_composition_t<N> & composition) -> bool
    requires(N == 1)
    {
        // a segment is not valid if the two vertices coincide
        if (composition[0]->footprint() == composition[1]->footprint()) {
            return false;
        }

        // a segment is not valid if the two vertices have same orientation
        if (composition[0]->orientation() == composition[1]->orientation()) {
            return false;
        }

        return true;
    }

    // TOFIX: remove these functions eventually
    // return the vertices of a segment following the order dictated by the segment's orientation
    auto vertices(const segment_t & segment) -> vertex_simplex_composition_t<1>
    {
        // a container to store a collection of vertices
        vertex_simplex_composition_t<1> vertices_collection;

        // the first two vertices come from the first edge
        vertices_collection[0] = segment->composition()[0]->footprint();
        vertices_collection[1] = segment->composition()[1]->footprint();

        // assert that you found two distinct vertices
        assert(vertices_collection[0] != vertices_collection[1]);

        // all done
        return vertices_collection;
    }

    // TOFIX: remove these functions eventually
    // return the vertices of a triangle following the order dictated by the triangle's
    // orientation
    auto vertices(const triangle_t & triangle) -> vertex_simplex_composition_t<2>
    {
        // a container to store a collection of vertices
        vertex_simplex_composition_t<2> vertices_collection;

        // get the three vertices following the orientation of the simplex
        const auto edge_0 = triangle->composition()[0];
        // the first two vertices come from the first edge
        vertices_collection[0] = edge_0->composition()[0]->footprint();
        vertices_collection[1] = edge_0->composition()[1]->footprint();
        // the third vertex comes from the second edge
        const auto edge_1 = triangle->composition()[1];
        vertices_collection[2] = edge_1->composition()[1]->footprint();

        // assert that you found three distinct vertices
        assert(vertices_collection[0] != vertices_collection[1]);
        assert(vertices_collection[1] != vertices_collection[2]);
        assert(vertices_collection[0] != vertices_collection[2]);

        // all done
        return vertices_collection;
    }

    // TOFIX: remove these functions eventually
    // return the vertices of a tetrahedron following the order dictated by the tetrahedron's
    // orientation
    auto vertices(const tetrahedron_t & tetrahedron) -> vertex_simplex_composition_t<3>
    {
        // get the vertices of the first face of the tetrahedron
        auto vertices_triangle_0 = vertices(tetrahedron->composition()[0]);

        // copy the vertices to the vertices collection of the tetrahedron
        vertex_simplex_composition_t<3> vertices_collection;
        vertices_collection[0] = vertices_triangle_0[0];
        vertices_collection[1] = vertices_triangle_0[1];
        vertices_collection[3] = vertices_triangle_0[2];

        // get the vertices of the second face of the tetrahedron
        auto vertices_triangle_1 = vertices(tetrahedron->composition()[1]);

        // TOFIX: for sure there is a better way to find the last vertex
        // get the remaining vertex
        vertices_collection[2] = (vertices_triangle_1[0] != vertices_collection[0]
                                  && vertices_triangle_1[0] != vertices_collection[1]
                                  && vertices_triangle_1[0] != vertices_collection[3]) ?
                                     vertices_triangle_1[0] :
                                     ((vertices_triangle_1[1] != vertices_collection[0]
                                       && vertices_triangle_1[1] != vertices_collection[1]
                                       && vertices_triangle_1[1] != vertices_collection[3]) ?
                                          vertices_triangle_1[1] :
                                          vertices_triangle_1[2]);

        // assert that you found four distinct vertices
        assert(vertices_collection[0] != vertices_collection[1]);
        assert(vertices_collection[0] != vertices_collection[2]);
        assert(vertices_collection[0] != vertices_collection[3]);
        assert(vertices_collection[1] != vertices_collection[2]);
        assert(vertices_collection[1] != vertices_collection[3]);
        assert(vertices_collection[2] != vertices_collection[3]);

        // all done
        return vertices_collection;
    }
}


#endif

// end of file
