// code guard
#if !defined(mito_mesh_metis_api_h)
#define mito_mesh_metis_api_h


namespace mito::mesh::metis {

    template <class meshT>
    auto partition(const meshT & mesh, int n_partitions, int n_rank) -> meshT
    {
        return Partitioner<meshT>::partition(mesh, n_partitions, n_rank);
    }
}


#endif

// end of file
