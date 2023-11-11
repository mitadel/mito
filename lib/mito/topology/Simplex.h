// code guard
#if !defined(mito_topology_Simplex_h)
#define mito_topology_Simplex_h

namespace mito::topology {

    /*
     * This class represents a Simplex of order N > 0.
     *
     * Simplex<N> is represented recursively as a collection of N+1 subsimplices of type
     * Simplex<N-1>.
     */

    template <int N>
    class Simplex : public utilities::Shareable {

        // vertex set alias
        using vertex_set_type = std::set<vertex_t>;

        // private constructors: only the SimplexFactory has the right to instantiate simplices
      private:
        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(const simplex_composition_t<N> & simplices) : _simplices(simplices) {}

        // constructor for a simplex based on its composition in terms of subsimplices
        constexpr Simplex(simplex_composition_t<N> && simplices) : _simplices(simplices) {}

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
        inline auto composition() const noexcept -> const simplex_composition_t<N> &
        {
            return _simplices;
        }

        // append the vertices of this simplex to a collection of vertices
        void vertices(VertexInsertable auto & vertices) const
        requires(N > 1)
        {
            for (const auto & simplex : composition()) {
                simplex->vertices(vertices);
            }
        }

        // append the vertices of this simplex to a collection of vertices
        void vertices(VertexInsertable auto & vertices) const
        requires(N == 1)
        {
            vertices.insert(_simplices[0]->footprint());
            vertices.insert(_simplices[1]->footprint());
        }

        auto vertices() const -> vertex_set_type
        {
            // use a set to cleanup duplicates
            vertex_set_type vertex_set;

            // populate the vertices
            vertices(vertex_set);

            // return the set of vertices
            return vertex_set;
        }

        // TOFIX
        // append the vertices of this simplex to a collection of vertices
        inline auto vertices2(std::vector<vertex_t> & vertices) const -> void
        requires(N > 1)
        {
            const auto & subsimplex0 = _simplices[0];

            //
            subsimplex0->_vertices2(vertices);

            // assert that {N} vertices were found
            assert(std::size(vertices) == N);

            const auto & subsimplex1 = _simplices[1];

            for (const auto & v : subsimplex1->footprint()->vertices()) {
                // if the vertex was not found in {vertices}
                auto found = std::find(std::begin(vertices), std::end(vertices), v);
                if (found == std::end(vertices)) {
                    vertices.push_back(v);
                }
            }

            // all done
            return;
        }

        // append the vertices of this simplex to a collection of vertices
        inline auto vertices2(std::vector<vertex_t> & vertices) const -> void
        requires(N == 1)
        {
            vertices.push_back(_simplices[0]->footprint());
            vertices.push_back(_simplices[1]->footprint());

            // all done
            return;
        }

        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        requires(N > 2)
        {
            // if N > 2, then recursively fetch the edges from the simplices in the composition
            for (const auto & simplex : composition()) {
                simplex->edges(edges);
            }

            // all done
            return;
        }

        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        requires(N == 2)
        {
            // if N == 2, then this simplex is a triangle and its composition already consists of
            // edges
            for (const auto & edge : composition()) {
                edges.insert(edge);
            }

            // all done
            return;
        }

        // perform a sanity check (check that a simplex of order N has N+1 distinct vertices)
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

            // collect vertices of this simplex
            auto vertices = this->vertices();

            // if this simplex does not have N+1 vertices, something went wrong
            if (std::size(vertices) != int(N) + 1) {
                // all done
                return false;
            }

            // all done
            return true;
        }

      private:
        // the simplex composition in terms of subsimplices
        const simplex_composition_t<N> _simplices;

        // private friendship with the repository of simplices
        friend class utilities::Repository<unoriented_simplex_t<N>>;
    };

    /*
     * This class collapses Simplex<N> for N = 0.
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
        friend class utilities::Repository<unoriented_simplex_t<0>>;
    };

}    // namespace mito


#endif    // mito_topology_Simplex_h

// end of file
