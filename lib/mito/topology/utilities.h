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
}


#endif

// end of file
