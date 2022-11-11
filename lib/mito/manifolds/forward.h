// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <int I, int D, template <int> class elementT>
    requires(I <= D)
    class Manifold;

    // manifold alias
    template <int I, int D, template <int> class elementT>
    requires(I <= D)
    using manifold_t = Manifold<I, D, elementT>;

    // vertex alias
    using vertex_t = topology::vertex_t;

    // segment alias
    using segment_t = topology::segment_t;

    // triangle alias
    using triangle_t = topology::triangle_t;

    // tetrahedron alias
    using tetrahedron_t = topology::tetrahedron_t;

    // vertex set alias
    using vertex_set_t = topology::vertex_set_t;

    // element set alias
    template <class elementT>
    using element_set_t = topology::element_set_t<elementT>;

    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;
}


#endif

// end of file
