// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // TOFIX
    // factory manifolds
    template <class cellT, int D>
    constexpr auto manifold(mesh::mesh_t<cellT, D> & mesh)
    {
        return manifold_t<cellT, D>(mesh);
    }

}


#endif

// end of file
