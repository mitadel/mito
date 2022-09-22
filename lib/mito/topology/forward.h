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
        using simplex_type = OrientedSimplex<0>;
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

    // oriented simplex composition alias
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_ptr<D - 1>, D + 1>;

    // simplex alias
    template <int D>
    using simplex_t = oriented_simplex_ptr<D>;

    // vertex alias
    using vertex_t = simplex_t<0>;

    // segment alias
    using segment_t = simplex_t<1>;

    // triangle alias
    using triangle_t = simplex_t<2>;

    // tetrahedron alias
    using tetrahedron_t = simplex_t<3>;

    // element set alias
    template <class elementT>
    using element_set_t = std::unordered_set<elementT>;

    // element vector alias
    template <class elementT>
    using element_vector_t = std::vector<elementT>;

    // vertex set alias
    using vertex_set_t = element_set_t<vertex_t>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;

    template <int D>
    using unoriented_simplex_id_t = std::uintptr_t;

    template <int D>
    using oriented_simplex_id_t = std::uintptr_t;
}


#endif

// end of file
