// code guard
#if !defined(mito_topology_forward_h)
#define mito_topology_forward_h


namespace mito::topology {

    // class simplex
    template <int D>
    requires(D > 0) class Simplex;

    // class oriented simplex
    template <int D>
    class OrientedSimplex;

    // class simplex factory
    template <int D>
    requires(D > 0) class SimplexFactory;

    // class oriented simplex factory
    template <int D>
    class OrientedSimplexFactory;

    // class oriented simplex factory
    template <int D>
    class Topology;

    // vertex alias
    using vertex_t = OrientedSimplex<0>;

    // helper class to allow template specialization of simplex alias
    template <int D>
    class helperSimplexClass {
      public:
        using simplex_type = Simplex<D>;
    };

    // helper class to allow template specialization of simplex alias
    template <>
    class helperSimplexClass<0> {
      public:
        using simplex_type = vertex_t;
    };

    // unoriented simplex alias
    template <int D>
    using unoriented_simplex_t = typename helperSimplexClass<D>::simplex_type;

    // unoriented simplex pointer alias
    template <int D>
    using unoriented_simplex_ptr = std::shared_ptr<const unoriented_simplex_t<D>>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = OrientedSimplex<D>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = std::shared_ptr<const OrientedSimplex<D>>;

    // element set alias
    template <class elementT>
    using element_set_t = std::unordered_set<std::shared_ptr<const elementT>>;

    // element vector alias
    template <class elementT>
    using element_vector_t = std::vector<std::shared_ptr<const elementT>>;

    // vertex set alias
    using vertex_set_t = element_set_t<vertex_t>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;

    // oriented simplex composition alias
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_ptr<D - 1>, D + 1>;

    // segment alias
    using segment_t = OrientedSimplex<1>;

    // triangle alias
    using triangle_t = OrientedSimplex<2>;

    // tetrahedron alias
    using tetrahedron_t = OrientedSimplex<3>;


}


#endif

// end of file
