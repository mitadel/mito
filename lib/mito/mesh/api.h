// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <class cellT, int D, geometry::CoordinateType coordT>
    auto mesh(geometry::geometry_t<D, coordT> & geometry) -> mesh_t<cellT, D, coordT>;
}


#endif

// end of file
