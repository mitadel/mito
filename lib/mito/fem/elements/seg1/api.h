// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // specialization of {finite_element_family} for first order segments
    template <int D>
    struct finite_element_family<geometry::segment_t<D>, 1> {

        // the dimension of the physical space
        static constexpr int dim = D;
        // the underlying mesh cell type
        using mesh_cell_type = geometry::segment_t<dim>;
        // the degree of the finite element
        static constexpr int degree = 1;
        // the number of nodes per element
        static constexpr int n_nodes = 2;

        // the discretization node type
        using discretization_node_type = discrete::discretization_node_t;
        // the connectivity type of the element
        using connectivity_type = std::array<discretization_node_type, n_nodes>;

        // the type of instances of this finite element
        template <class parametrizedElementT>
        using type = IsoparametricSegmentP1<parametrizedElementT>;
    };

}


// end of file
