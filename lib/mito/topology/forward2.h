// code guard
#if !defined(mito_topology_forward2_h)
#define mito_topology_forward2_h


namespace mito::topology {

    // alias for oriented simplex recursive composition
    // (e.g. a N-simplex has N+1 (N-1)-simplices)
    template <int N>
    using simplex_composition_t = std::array<oriented_simplex_t<N - 1>, N + 1>;

    // alias for oriented simplex composition in terms of vertices
    // (e.g. a N-simplex has N+1 0-simplices, i.e. vertices)
    template <int N>
    using vertex_simplex_composition_t = std::array<vertex_t, N + 1>;

    // alias for the collection of oriented simplex director vectors
    // (e.g. an N-simplex has as N director vectors)
    template <int N, int D>
    using edge_simplex_directors_t = std::array<vector_t<D>, N>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;

    // concept for a class supporting the {insert} method for a {vertex_t} argument
    template <class T>
    concept VertexInsertable = requires(T instance, const vertex_t & v) { instance.insert(v); };

    // concept for a class supporting the {push_back} method for a {vertex_t} argument
    template <class T>
    concept VertexPushBackable =
        requires(T instance, const vertex_t & v) { instance.push_back(v); };
}


#endif

// end of file
