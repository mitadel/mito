// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <class cellT, int D>
    auto mesh(mito::geometry::geometry_t<D> & geometry) -> mesh_t<cellT, D>;
}


#endif

// end of file
