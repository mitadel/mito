// code guard
#if !defined(mito_topology_forward1_h)
#define mito_topology_forward1_h


namespace mito::topology {

    // class simplex
    template <int N>
    class Simplex;

    // class oriented simplex
    template <int N>
    class OrientedSimplex;

    // class simplex factory
    template <int N>
    class SimplexFactory;

    // simplex factory alias
    template <int N>
    using simplex_factory_t = SimplexFactory<N>;

    // class oriented simplex factory
    template <int N>
    class OrientedSimplexFactory;

    // oriented simplex factory alias
    template <int N>
    using oriented_simplex_factory_t = OrientedSimplexFactory<N>;

    // class topology
    class Topology;

    // topology alias
    using topology_t = Topology;

    // TOFIX: not sure if this type is useful in other places than {Mesh}
    // element set alias
    template <class cellT>
    using element_set_t = std::unordered_set<cellT, utilities::hash_function<cellT>>;

    // unoriented simplex alias
    template <int N>
    using unoriented_simplex_t = utilities::shared_ptr<const Simplex<N>>;

    // oriented simplex alias
    template <int N>
    using oriented_simplex_t = utilities::shared_ptr<const OrientedSimplex<N>>;

    // type for simplex orientation
    using orientation_t = int;
}


#endif

// end of file
