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
        constexpr Simplex(const std::array<simplex_t<D - 1> *, D + 1> & simplices) :
            _simplices(simplices)
        {
            // initialize object
            _initialize();

            // all done
            return;
        }

        constexpr Simplex(std::array<simplex_t<D - 1> *, D + 1> && simplices) : _simplices(simplices)
        {
            // initialize object
            _initialize();

            // all done
            return;
        }

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

      private:
        void _initialize()
        {
            // sort the simplices (using the address of the simplices) so that two simplices
            // having the same simplices will result in two identical instances of class Simplex
            std::sort(_simplices.begin(), _simplices.end());

            // all done
            return;
        }

      public:
        const auto & simplices() const { return _simplices; }

        void vertices(std::set<const vertex_t *> & vertices) const
        {
            for (const auto & simplex : simplices()) {
                simplex->vertices(vertices);
            }
        }

      private:
        std::array<simplex_t<D - 1> *, D + 1> _simplices;
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
