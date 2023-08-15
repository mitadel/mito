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
    class Simplex : public utilities::Shareable {

        // private constructors: only the SimplexFactory has the right to instantiate simplices
      private:
        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(const simplex_composition_t<D> & simplices) : _simplices(simplices) {}

        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(simplex_composition_t<D> && simplices) : _simplices(simplices) {}

        // destructor
        constexpr ~Simplex() override {}

      private:
        // delete default constructor
        Simplex() = delete;

        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(Simplex &&) = delete;

        // delete assignment operator
        Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        Simplex & operator=(Simplex &&) = delete;

      public:
        // accessor for the subsimplices
        inline auto composition() const noexcept -> const simplex_composition_t<D> &
        {
            return _simplices;
        }

        // append the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        requires(D > 1)
        {
            for (const auto & simplex : composition()) {
                simplex->vertices(vertices);
            }
        }

        // append the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        requires(D == 1)
        {
            vertices.insert(_simplices[0]->footprint());
            vertices.insert(_simplices[1]->footprint());
        }

        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        requires(D > 2)
        {
            // if D > 2, then recursively fetch the edges from the simplices in the composition
            for (const auto & simplex : composition()) {
                simplex->edges(edges);
            }

            // all done
            return;
        }

        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        requires(D == 2)
        {
            // if D == 2, then this simplex is a triangle and its composition already consists of
            // edges
            for (const auto & edge : composition()) {
                edges.insert(edge);
            }

            // all done
            return;
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
            if (std::size(vertices) != int(D) + 1) {
                // all done
                return false;
            }

            // all done
            return true;
        }

      private:
        // the simplex composition in terms of subsimplices
        const simplex_composition_t<D> _simplices;

        // private friendship with the repository of simplices
        friend class utilities::Repository<Simplex<D>>;
    };

    /*
     * This class collapses Simplex<D> for D = 0.
     *
     * A simplex of order 0, like a vertex, is an empty object.
     */

    template <>
    class Simplex<0> : public utilities::Shareable {
      private:
        // default constructor
        constexpr Simplex() {}

        // empty destructor
        constexpr ~Simplex() override {}

      private:
        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(Simplex &&) = delete;

        // delete assignment operator
        Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        Simplex & operator=(Simplex &&) = delete;

      public:
        // perform a sanity check
        inline auto sanityCheck() const noexcept -> bool
        {
            // a simplex of order 0 has only 1 vertex (this one!)
            return true;
        }

      private:
        // friendship with the repository of simplices
        friend class utilities::Repository<Simplex<0>>;
    };

}    // namespace mito


#endif    // mito_topology_Simplex_h

// end of file
