// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h


namespace mito::mesh::metis {

    template <class meshT, geometry::CoordinateType coordT>
    class Partitioner {

      private:
        // typedef my template parameter
        using mesh_type = meshT;
        // typedef cell type
        using cell_type = typename mesh_type::cell_type;
        // typedef vertex type
        using vertex_type = topology::vertex_t;
        // typedef node type
        using node_type = typename mesh_type::node_type;
        // typedef coordinate system type
        using coordinate_system_type = geometry::coordinate_system_t<mesh_type::dim, coordT>;

      private:
        // populate a map between vertices and a continuous integer id = 0, ..., n_vertices - 1
        static inline auto _populate_vertices_map(const mesh_type & mesh) -> auto;
        // get the mesh cells in with a lexicographic order on the barycenter
        static inline auto _get_ordered_mesh_cells(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system) -> auto;
        // populate elements connectivity
        static inline auto _populate_element_connectivity(
            const auto & cells, const auto & vertex_to_id,
            const coordinate_system_type & coordinate_system) -> auto;
        // paint metis partition
        static inline auto _metis_paint_partition(
            std::vector<int> & element_connectivity, int n_vertices, int n_elements,
            int n_partitions) -> auto;
        // return a partitioned mesh with the painted partition
        static inline auto _create_partitioned_mesh(
            const auto & cells, const auto & painting, int n_rank) -> mesh_type;

      public:
        // paint partition and return the partition corresponding to {n_rank}
        static inline auto partition(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system,
            int n_partitions, int n_rank) -> mesh_type;
    };
}


// get the inline definitions
#define mito_mesh_metis_partitioner_icc
#include "Partitioner.icc"
#undef mito_mesh_metis_partitioner_icc


#endif

// end of file
