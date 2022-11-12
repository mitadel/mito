// code guard
#if !defined(mito_topology_forward_h)
#define mito_topology_forward_h


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

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = mito::utilities::shared_ptr<oriented_simplex_t<D>>;

    // id type of unoriented simplex
    using unoriented_simplex_id_t = std::uintptr_t;

    // id type of unoriented simplex
    // QUESTION: should we collapse these two ids and call them {element_id_t}?
    using oriented_simplex_id_t = std::uintptr_t;

    // oriented simplex composition alias
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_ptr<D - 1>, D + 1>;

    // simplex alias
    template <int D>
    using simplex_t = oriented_simplex_ptr<D>;

    // hash function for {elementT}, which is a (shared) pointer to an element
    // Note that two pointers pointing to the same element collapse on the same hashed value
    template <class elementT>
    struct element_hash {
        size_t operator()(const elementT & element) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<mito::topology::unoriented_simplex_id_t>(
                static_cast<elementT::handle_t>(element.handle()));
        }
    };

    // element set alias
    template <class elementT>
    using element_set_t = std::unordered_set<elementT, element_hash<elementT>>;

    // element vector alias
    template <class elementT>
    using element_vector_t = std::vector<elementT>;

    // a collection of elements
    // QUESTION: this data structure should only be used by the factories, so where should it be
    //              typedef'ed?
    template <class elementT>
    using element_collection_t =
        mito::utilities::segmented_t<typename elementT::resource_t, 100 /* segment size */>;
}


#endif

// end of file
