// code guard
#if !defined(mito_geometry_GeometricSimplex_h)
#define mito_geometry_GeometricSimplex_h

/*
 * This class represents an oriented simplex of order N in D-dimensional space.
 *
 * Each instance of {GeometricSimplex} consists of an underlying {OrientedSimplex} and a
 * vertex-point mapping.
 *
 */

namespace mito::geometry {

    template <int N, int D>
    // requires((N > 0) && (N <= D) && (D > 0))
    class GeometricSimplex {

      public:
        // spatial dimension
        static constexpr int dim = D;

        // order of simplex
        static constexpr int order = N;

        // the type of the vertices
        using vertex_type = mito::topology::vertex_t;

        // the type of the topological simplex
        using simplex_type = mito::topology::oriented_simplex_t<N>;

        // number of vertices of simplex
        static constexpr int n_vertices = topology::n_vertices<simplex_type>();

        // the node type
        using node_type = node_t<D>;

        // a collection of nodes
        using nodes_type = std::array<node_type, N + 1>;

      private:
        template <int... J>
        constexpr auto _check_vertices(integer_sequence<J...>) const -> void
        {
            assert(
                mito::math::permutation_sign(_simplex->vertices(), { _nodes[J].vertex()... })
                == +1);
        }

      public:
        // constructor with an existing oriented simplex and a collection of nodes
        constexpr GeometricSimplex(const simplex_type & simplex, const nodes_type & nodes) :
            _simplex(simplex),
            _nodes(nodes)
        {
            // check that the vertices in {nodes} match the vertices of the {simplex} within a
            // positive permutation
            _check_vertices(make_integer_sequence<N + 1> {});
        }

        // move constructor
        GeometricSimplex(GeometricSimplex &&) = default;

        // copy constructor
        GeometricSimplex(const GeometricSimplex &) = default;

        // destructor
        constexpr ~GeometricSimplex() {}

      private:
        // delete default constructor
        GeometricSimplex() = delete;

        // delete assignment operator
        GeometricSimplex & operator=(const GeometricSimplex &) = delete;

        // delete move assignment operator
        GeometricSimplex & operator=(GeometricSimplex &&) = delete;

      public:
        // accessor for the underlying oriented simplex
        inline auto simplex() const noexcept -> const simplex_type & { return _simplex; }

        // return the composition of this simplex in terms of its vertices
        inline auto nodes() const -> nodes_type { return _nodes; }

      private:
        // the shared pointer to the footprint
        const simplex_type _simplex;
        // the simplex nodes
        const nodes_type _nodes;
    };
}


#endif    // mito_geometry_GeometricSimplex_h

// end of file
