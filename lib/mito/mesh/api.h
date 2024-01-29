// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <class cellT>
    auto mesh() -> mesh_t<cellT>;
}


#endif

// end of file
