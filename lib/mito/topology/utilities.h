// code guard
#if !defined(mito_topology_utilities_h)
#define mito_topology_utilities_h


namespace mito::topology {
    // overload operator<< for oriented simplices
    template <int D>
    std::ostream & operator<<(std::ostream & os, const simplex_t<D> & s)
    {
        // print orientation
        os << "orientation: " << s->orientation() << std::endl;
        // print footprint
        os << "footprint: " << s->footprint() << std::endl;
        // all done
        return os;
    }

    // overload operator<< for oriented simplices
    template <>
    std::ostream & operator<<(std::ostream & os, const simplex_t<0> & s)
    {
        os << "vertex: " << s->footprint().handle() << std::endl;
        return os;
    }

    auto tail(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[0]->footprint();
        } else {
            return oriented_simplex->composition()[1]->footprint();
        }
    }

    auto head(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[1]->footprint();
        } else {
            return oriented_simplex->composition()[0]->footprint();
        }
    }

    bool headTailConnected(const simplex_t<1> & simplex_1, const simplex_t<1> & simplex_2)
    {
        if (head(simplex_1) == tail(simplex_2)) {
            return true;
        }

        return false;
    }

    // what would be the 2D implementation of this function?
    auto isValid([[maybe_unused]] const simplex_composition_t<3> & composition) -> bool
    {
        return true;
    }

    // a triangle is valid if the edges are all head-tail connected
    auto isValid([[maybe_unused]] const simplex_composition_t<2> & composition) -> bool
    {
        if (headTailConnected(composition[0], composition[1])
            && headTailConnected(composition[1], composition[2])
            && headTailConnected(composition[2], composition[0])) {
            return true;
        }

        return false;
    }

    auto isValid(const simplex_composition_t<1> & composition) -> bool
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

    // return the vertices of a triangle following the order dictated by the triangle's orientation
    auto vertices(const triangle_t & triangle) -> vertex_simplex_composition_t<3>
    {
        // a container to store a collection of vertices
        vertex_simplex_composition_t<3> vertices_collection;

        // get the three vertices following the orientation of the simplex
        const auto & edge_0 = triangle->composition()[0];
        // the first two vertices come from the first edge
        vertices_collection[0] = edge_0->orientation() ? edge_0->composition()[0]->footprint() :
                                                         edge_0->composition()[1]->footprint();
        vertices_collection[1] = edge_0->orientation() ? edge_0->composition()[1]->footprint() :
                                                         edge_0->composition()[0]->footprint();
        // the third vertex comes from the second edge
        const auto & edge_1 = triangle->composition()[1];
        vertices_collection[2] = edge_1->orientation() ? edge_1->composition()[1]->footprint() :
                                                         edge_1->composition()[0]->footprint();

        // assert that you found three distinct vertices
        assert(vertices_collection[0] != vertices_collection[1]);
        assert(vertices_collection[1] != vertices_collection[2]);
        assert(vertices_collection[0] != vertices_collection[2]);

        // all done
        return vertices_collection;
    }
}


#endif

// end of file
