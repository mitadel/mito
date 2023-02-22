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
        os << "footprint: " << s->footprint().handle() << std::endl;
        // all done
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
}


#endif

// end of file
