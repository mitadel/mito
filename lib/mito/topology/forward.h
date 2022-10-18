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

    // vertex alias
    using vertex_t = simplex_t<0>;

    // segment alias
    using segment_t = simplex_t<1>;

    // triangle alias
    using triangle_t = simplex_t<2>;

    // tetrahedron alias
    using tetrahedron_t = simplex_t<3>;

    // hash function for {elementT}, which is a (shared) pointer to an element
    // Note that two pointers pointing to the same element collapse on the same hashed value 
    template <class elementT>
    struct element_hash {
       size_t operator() (const elementT &element) const {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<mito::topology::unoriented_simplex_id_t>(static_cast<elementT::handle_t>(element));
       }
    };

    // element set alias
    template <class elementT>
    using element_set_t = std::unordered_set<elementT, element_hash<elementT>>;

    // element vector alias
    template <class elementT>
    using element_vector_t = std::vector<elementT>;

    // vertex set alias
    using vertex_set_t = element_set_t<vertex_t>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;

    // a collection of elements 
    // QUESTION: this data structure should only be used by the factories, so where should it be 
    //              typedef'ed?
    template <class elementT>
    using element_collection_t = mito::utilities::segmented_t<typename elementT::resource_t, 100 /* segment size */>;
}


#endif

// end of file
