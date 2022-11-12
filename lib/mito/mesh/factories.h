// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // mesh factory
    template <int D, template <int> class cellT>
    auto mesh() -> mesh_t<D, cellT>
    {
        return mesh_t<D, cellT>();
    }

}


#endif

// end of file
