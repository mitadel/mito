// code guard
#if !defined(mito_topology_Simplex_h)
#define mito_topology_Simplex_h

namespace mito::topology {

    /*
     * This class represents a Simplex of order D > 0.
     *
     * Simplex<D> is represented recursively as a collection of D+1 subsimplices of type
     * Simplex<D-1>.
     */

    template <int D>
    class Simplex : public mito::utilities::Shareable {

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
        inline auto composition() const -> const auto & { return _simplices; }

        // returns the simplex id
        inline auto id() const -> unoriented_simplex_id_t
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t>(this);
        }

        // add the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        requires(D > 1)
        {
            for (const auto & simplex : composition()) {
                simplex->vertices(vertices);
            }
        }

        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        requires(D == 1)
        {
            vertices.insert(_simplices[0]->footprint());
            vertices.insert(_simplices[1]->footprint());
        }

        // perform a sanity check (check that a simplex of order D has D+1 distinct vertices)
        inline auto sanityCheck() const -> bool
        {
            // check the subsimplices
            for (const auto & simplex : composition()) {
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
        inline auto _erase() -> void
        {
            // reset the subsimplices shared pointers
            for (auto & simplex : _simplices) {
                simplex.reset();
            }

            // all done
            return;
        }

      private:
        // the simplex composition in terms of subsimplices
        simplex_composition_t<D> _simplices;    // TOFIX: should this be {const}?

        // private friendship with the factory of simplices
        friend class SimplexFactory<D>;
    };

    /*
     * This class collapses Simplex<D> for D = 0.
     *
     * A simplex of order 0, like a vertex, is an empty object.
     */

    template <>
    class Simplex<0> : public mito::utilities::Shareable {
      private:
        // default constructor
        constexpr Simplex() {}

      public:
        // empty destructor
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
        // returns the simplex id
        inline auto id() const -> unoriented_simplex_id_t
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t>(this);
        }

        // perform a sanity check
        inline auto sanityCheck() const -> bool
        {
            // a simplex of order 0 has only 1 vertex (this one!)
            return true;
        }

      private:
        inline auto _erase() -> void
        {
            // all done
            return;
        }

      private:
        // friendship with the factory of simplices
        friend class SimplexFactory<0>;
    };

}    // namespace mito


#endif    // mito_topology_Simplex_h

// end of file
