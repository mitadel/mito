// code guard
#if !defined(mito_mesh_Simplex_h)
#define mito_mesh_Simplex_h

namespace mito::mesh {

    /*
     * This class represents a Simplex of order D > 0.
     *
     * Simplex<D> is represented recursively as a collection of D+1 subsimplices of type
     * OrientedSimplex<D-1>.
     */

    template <int D>
    requires(D > 0) class Simplex {

        // private constructors: only the SimplexFactory has the right to instantiate simplices
      private:
        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(const simplex_composition_t<D> & simplices) : _simplices(simplices) {}

        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(simplex_composition_t<D> && simplices) : _simplices(simplices) {}

      public:
        // destructor
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
        // accessor for the subsimplices
        const auto & simplices() const { return _simplices; }

        // add the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const requires(D > 1)
        {
            for (const auto & simplex : simplices()) {
                simplex->vertices(vertices);
            }
        }

        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const requires(D == 1)
        {
            vertices.insert(_simplices[0]);
            vertices.insert(_simplices[1]);
        }

        // perform a sanity check (check that a simplex of order D has D+1 distinct vertices)
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
            vertex_set_t vertices;
            // collect vertices of this simplex
            this->vertices(vertices);

            // if this simplex does not have D+1 vertices, something went wrong
            if (vertices.size() != int(D) + 1) {
                // all done
                return false;
            }

            // all done
            return true;
        }

      private:
        // the simplex composition in terms of subsimplices
        simplex_composition_t<D> _simplices;

        // private friendship with the factory of simplices
        friend class SimplexFactory<D>;
    };

    // overload operator<< for simplices
    template <int D>
    std::ostream & operator<<(std::ostream & os, const Simplex<D> & s) requires(D > 0)
    {
        os << &s << " composed of:" << std::endl;
        for (const auto & simplex : s.simplices()) {
            std::cout << "\t" << *simplex << std::endl;
        }
        return os;
    }


}    // namespace mito


#endif    // mito_mesh_Simplex_h

// end of file
