// code guard
#if !defined(mito_mesh_Simplex_h)
#define mito_mesh_Simplex_h

/*
 * This class represents a class of equivalence for a Simplex of dimension D. 
 *
 * Simplex<D> is represented recursively as a collection of D+1 Simplex<D-1>. An instance of 
 * Simplex<D> represents therefore (D+1)! simplices, depending on the (D+1)! possible orderings of 
 * the subsimplices.
 *
 * The representative in this class of equivalence is chosen by sorting in increasing order the 
 * addresses pointing to the subsimplices.  
 */

namespace mito::mesh {

    template <int D>
    class Simplex {

      public:
        constexpr Simplex(const simplex_composition_t<D> & simplices) : _simplices(simplices) {}

        constexpr Simplex(simplex_composition_t<D> && simplices) : _simplices(simplices) {}

        constexpr ~Simplex() {}

      private:
        // delete default constructor
        Simplex() = delete;

        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(const Simplex &&) = delete;

        // delete assignment operator
        const Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        const Simplex & operator=(const Simplex &&) = delete;

      public:
        const auto & simplices() const { return _simplices; }

        void vertices(std::set<const vertex_t *> & vertices) const
        {
            for (const auto & simplex : simplices()) {
                simplex->vertices(vertices);
            }
        }

        bool sanityCheck() const
        {
            // check the subsimplices
            for (const auto & simplex : simplices()) {
                // if a subsimplex is broken, the sanity check fails
                if (!simplex->sanityCheck()) {
                    // all done
                    return false;
                }
            }

            // use a set to cleanup duplicates
            std::set<const vertex_t *> vertices;
            // collect vertices of every subsimplex of this simplex
            for (const auto & simplex : simplices()) {
                simplex->vertices(vertices);
            }
            // if this simplex does not have D+1 vertices, something went wrong
            if (vertices.size() != int(D) + 1) {
                // all done
                return false;
            }

            // all done
            return true;
        }

      private:
        simplex_composition_t<D> _simplices;
    };

    template <>
    class Simplex<0> {
      public:
        constexpr Simplex() {}
        constexpr ~Simplex() {}

      private:
        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(const Simplex &&) = delete;

        // delete assignment operator
        const Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        const Simplex & operator=(const Simplex &&) = delete;

      public:
        void vertices(std::set<const vertex_t *> & vertices) const
        {
            // insert this vertex
            vertices.insert(this);
            // all done
            return;
        }

      public:
        bool sanityCheck() const { return true; }
    };

    // overload operator<< for simplices
    template <int D>
    std::ostream & operator<<(std::ostream & os, const mito::mesh::simplex_t<D> & s)
    {
        os << &s << " composed of:" << std::endl;
        for (const auto & simplex : s.simplices()) {
            std::cout << "\t" << *simplex << std::endl;
        }
        return os;
    }

    // overload operator<< specialization for simplices with D = 0 (vertices)
    template <>
    std::ostream & operator<<(std::ostream & os, const mito::mesh::simplex_t<0> & s)
    {
        os << &s;
        return os;
    }

}    // namespace mito


#endif    // mito_mesh_Simplex_h

// end of file
