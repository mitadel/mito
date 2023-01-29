// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <int D, template <int> class cellT, int N>
    auto mesh(mito::geometry::geometry_t<D> & geometry) -> mesh_t<D, cellT, N>;
}


#endif

// end of file
