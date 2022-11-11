// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // mesh factory
    template <int D, template <int> class elementT>
    auto mesh() -> mesh_t<D, elementT>
    {
        return mesh_t<D, elementT>();
    }

}


#endif

// end of file
