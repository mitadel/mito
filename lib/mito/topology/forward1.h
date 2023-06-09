// code guard
#if !defined(mito_topology_forward1_h)
#define mito_topology_forward1_h


namespace mito::topology {

    // class simplex
    template <int D>
    class Simplex;

    // class oriented simplex
    template <int D>
    class OrientedSimplex;

    // class simplex factory
    template <int D>
    class SimplexFactory;

    // simplex factory alias
    template <int D>
    using simplex_factory_t = SimplexFactory<D>;

    // class oriented simplex factory
    template <int D>
    class OrientedSimplexFactory;

    // oriented simplex factory alias
    template <int D>
    using oriented_simplex_factory_t = OrientedSimplexFactory<D>;

    // class topology
    class Topology;

    // topology alias
    using topology_t = Topology;

    // TOFIX: not sure if this type is useful in other places than {Mesh}
    // element set alias
    template <class cellT>
    using element_set_t = std::unordered_set<cellT, utilities::hash_function<cellT>>;

    // TOFIX: this is mostly used in {Manifold}: either remove it or {typedef} there
    // element vector alias
    template <class cellT>
    using element_vector_t = std::vector<cellT>;

    // unoriented simplex alias
    template <int D>
    using unoriented_simplex_t = utilities::shared_ptr<Simplex<D>>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = utilities::shared_ptr<OrientedSimplex<D>>;

    // id type of unoriented simplex
    template <int D>
    using unoriented_simplex_id_t = utilities::index_t<unoriented_simplex_t<D>>;

    // id type of unoriented simplex
    template <int D>
    using oriented_simplex_id_t = utilities::index_t<oriented_simplex_t<D>>;
}


#endif

// end of file
