// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // mesh factory
    template <int D, template <int> class cellT, int N = D>
    auto mesh(
        mito::topology::topology_t & topology, mito ::geometry::point_cloud_t<D> & point_cloud)
        -> mesh_t<D, cellT, N>;
}


#endif

// end of file
