// code guard
#if !defined(mito_mesh_metis_api_h)
#define mito_mesh_metis_api_h


namespace mito::mesh::metis {

    template <class meshT, geometry::CoordinateType coordT>
    auto partition(
        const meshT & mesh,
        const geometry::coordinate_system_t<meshT::dim, coordT> & coordinate_system,
        int n_partitions, int n_rank) -> meshT
    {
        return Partitioner<meshT, coordT>::partition(mesh, coordinate_system, n_partitions, n_rank);
    }
}


#endif

// end of file
