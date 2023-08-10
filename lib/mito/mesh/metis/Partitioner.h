// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h


namespace mito::mesh::metis {

    template <class meshT>
    class Partitioner {

      private:
        // typedef my template parameter
        using mesh_t = meshT;
        // typedef vertex type
        using vertex_t = mito::topology::vertex_t;
        // typedef edge type
        using edge_t = mito::topology::segment_t;

      private:
        // populate a map between vertices and a continuous integer id = 0, ..., n_vertices - 1
        static auto _populate_vertices_map(const mesh_t & mesh) -> auto;
        // get the mesh cells in with a lexicographic order on the barycenter
        static auto _get_ordered_mesh_cells(const mesh_t & mesh) -> auto;
        // populate elements connectivity
        static auto _populate_element_connectivity(
            const auto & cells, const auto & vertex_to_id,
            const mito::geometry::geometry_t<mesh_t::dim> & geometry) -> auto;
        // paint metis partition
        static auto _metis_paint_partition(
            std::vector<int> & element_connectivity, int n_vertices, int n_elements,
            int n_partitions) -> std::vector<idx_t>;

      public:
        // paint partition and return the partition corresponding to {n_rank}
        static auto partition(const mesh_t & mesh, int n_partitions, int n_rank) -> mesh_t;
    };
}


// get the inline definitions
#define mito_mesh_metis_partitioner_icc
#include "Partitioner.icc"
#undef mito_mesh_metis_partitioner_icc


#endif

// end of file
