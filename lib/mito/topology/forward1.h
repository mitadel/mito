// code guard
#if !defined(mito_topology_forward1_h)
#define mito_topology_forward1_h


namespace mito::topology {

    // class simplex
    template <int D>
    requires(D > 0)
    class Simplex;

    // class oriented simplex
    template <int D>
    class OrientedSimplex;

    // class simplex factory
    template <int D>
    requires(D > 0)
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

    // unoriented simplex alias
    template <int D>
    requires(D > 0)
    using unoriented_simplex_t = Simplex<D>;

    // unoriented simplex pointer alias
    template <int D>
    using unoriented_simplex_ptr = mito::utilities::shared_ptr<unoriented_simplex_t<D>>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = OrientedSimplex<D>;

    // id type of unoriented simplex
    using unoriented_simplex_id_t = std::uintptr_t;

    // id type of unoriented simplex
    // QUESTION: should we collapse these two ids and call them {cell_id_t}?
    using oriented_simplex_id_t = std::uintptr_t;

    // hash function for {cellT}, which is a (shared) pointer to a cell
    // Note that two pointers pointing to the same cell collapse on the same hashed value
    template <class cellT>
    struct cell_hash {
        size_t operator()(const cellT & cell) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<mito::topology::unoriented_simplex_id_t>(
                static_cast<cellT::handle_t>(cell.handle()));
        }
    };

    // TOFIX: not sure if this type is useful in other places than {Mesh}
    // element set alias
    template <class cellT>
    using element_set_t = std::unordered_set<cellT, cell_hash<cellT>>;

    // TOFIX: this is mostly used in {Manifold}: either remove it or {typedef} there
    // element vector alias
    template <class cellT>
    using element_vector_t = std::vector<cellT>;

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = mito::utilities::shared_ptr<oriented_simplex_t<D>>;

    // simplex alias
    template <int D>
    using simplex_t = oriented_simplex_ptr<D>;
}


#endif

// end of file
