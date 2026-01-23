// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::mesh {

    template <class meshT, int I>
    class Filter {

      private:
        // typedef my template parameter
        using mesh_type = meshT;
        // typedef cell type
        using cell_type = typename mesh_type::cell_type;
        // the dimension of physical space
        static constexpr int D = cell_type::dim;
        // get the order of the cell
        static constexpr int N = cell_type::order;
        // a collection of nodes
        using nodes_type = cell_type::nodes_type;
        // get the (topological) family type that this cell type belongs to (e.g. simplicial cells)
        template <int J>
        using cell_topological_family_type = typename cell_type::cell_topological_family_type<J>;
        // construct the (geometric) cell type that filtered cells belong to (e.g. geometric
        // simplex)
        using filtered_cell_type = typename cell_type::cell_family_type<I, D>;
        // the filtered mesh type
        using mesh_filtered_type = mesh_t<filtered_cell_type>;

      public:
        // returns the mesh of the I-cells that compose {mesh}
        static inline auto filter(const mesh_type & mesh) -> mesh_filtered_type
        requires(I < N);

      private:
        template <int J>
        static inline auto _insert_filtered_subcells(
            mesh_filtered_type & filtered_mesh, const cell_topological_family_type<J> & cell,
            const nodes_type & nodes) -> void
        requires(I == J);

        template <int J>
        static inline auto _insert_filtered_subcells(
            mesh_filtered_type & filtered_mesh, const cell_topological_family_type<J> & cell,
            const nodes_type & nodes) -> void
        requires(I < J);
    };
}


// get the inline definitions
#define mito_mesh_filter_icc
#include "Filter.icc"
#undef mito_mesh_filter_icc


// end of file
