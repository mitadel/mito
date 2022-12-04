// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifolds
    template <int I, int D, template <int> class elementT>
    requires(I <= D)
    constexpr auto manifold(const mesh::mesh_t<D, elementT> & mesh)
    {
        return manifold_t<I, D, elementT>(mesh);
    }

}


#endif

// end of file
