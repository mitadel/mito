// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h


namespace mito::mesh::metis {

    template <class meshT>
    class Partitioner {

      private:
        // typedef for mesh type
        using mesh_t = meshT;

      public:
        static auto partition(const mesh_t & mesh, int n_partitions, int n_rank) -> mesh_t;
    };
}


// get the inline definitions
#define mito_mesh_metis_partitioner_icc
#include "partitioner.icc"
#undef mito_mesh_metis_partitioner_icc


#endif

// end of file
