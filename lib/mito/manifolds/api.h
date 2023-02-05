// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, int D>
    constexpr auto manifold(mesh::mesh_t<cellT, D> & mesh);
}


#endif

// end of file
