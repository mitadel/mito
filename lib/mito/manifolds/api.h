// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, int D>
    constexpr auto manifold(mesh::mesh_t<cellT, D> & mesh);

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

}


#endif

// end of file
