// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // mesh factory
    template <class cellT, int D, geometry::CoordinateSystem coordT>
    auto mesh(geometry::geometry_t<D, coordT> & geometry) -> mesh_t<cellT, D, coordT>
    {
        return mesh_t<cellT, D, coordT>(geometry);
    }

}


#endif

// end of file
