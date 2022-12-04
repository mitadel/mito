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

    // overload operator<< specialization for simplices with D = 0 (vertices)
    template <>
    std::ostream & operator<<(std::ostream & os, const simplex_t<0> & s)
    {
        os << s->footprint_id();
        return os;
    }

    auto tail(const simplex_t<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[0];
        } else {
            return oriented_simplex->composition()[1];
        }
    }

    auto head(const simplex_t<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[1];
        } else {
            return oriented_simplex->composition()[0];
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

    // a segment is valid if the two vertices are not the same
    auto isValid(const simplex_composition_t<1> & composition) -> bool
    {
        if (composition[0] != composition[1]) {
            return true;
        }

        return false;
    }
}


#endif

// end of file
