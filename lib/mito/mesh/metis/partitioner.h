// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h


namespace mito::mesh::metis {

    template <class meshT>
    auto partition(const meshT & mesh, int n_partitions, int n_rank) -> meshT;
}


// get the inline definitions
#define mito_mesh_metis_partitioner_icc
#include "partitioner.icc"
#undef mito_mesh_metis_partitioner_icc


#endif

// end of file
