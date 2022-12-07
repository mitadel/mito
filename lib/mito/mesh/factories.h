// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // mesh factory
    template <int D, template <int> class cellT, int N>
    auto mesh(
        mito::topology::topology_t & topology, mito ::geometry::point_cloud_t<D> & point_cloud)
        -> mesh_t<D, cellT, N>
    {
        return mesh_t<D, cellT, N>(topology, point_cloud);
    }

}


#endif

// end of file
