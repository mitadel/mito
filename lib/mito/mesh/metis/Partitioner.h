// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h


namespace mito::mesh::metis {

    template <class meshT, geometry::CoordinateType coordT>
    class Partitioner {

      private:
        // typedef my template parameter
        using mesh_type = meshT;
        // typedef vertex type
        using vertex_type = topology::vertex_t;
        // typedef geometry type
        using geometry_type = geometry::geometry_t<mesh_type::dim>;
        // typedef coordinate system type
        using coordinate_system_type = geometry::coordinate_system_t<mesh_type::dim, coordT>;

      private:
        // populate a map between vertices and a continuous integer id = 0, ..., n_vertices - 1
        static auto _populate_vertices_map(const mesh_type & mesh) -> auto;
        // get the mesh cells in with a lexicographic order on the barycenter
        static auto _get_ordered_mesh_cells(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system) -> auto;
        // populate elements connectivity
        static auto _populate_element_connectivity(
            const auto & cells, const auto & vertex_to_id, const geometry_type & geometry,
            const coordinate_system_type & coordinate_system) -> auto;
        // paint metis partition
        static auto _metis_paint_partition(
            std::vector<int> & element_connectivity, int n_vertices, int n_elements,
            int n_partitions) -> auto;
        // return a partitioned mesh with the painted partition
        static auto _create_partitioned_mesh(
            const mesh_type & mesh, const auto & cells, const auto & painting, int n_rank)
            -> mesh_type;

      public:
        // paint partition and return the partition corresponding to {n_rank}
        static auto partition(
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
