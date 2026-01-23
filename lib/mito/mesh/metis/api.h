// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::mesh::metis {

    template <class meshT>
    auto partition(const meshT & mesh, int n_partitions, int n_rank) -> meshT
    {
        return Partitioner<meshT>::partition(mesh, n_partitions, n_rank);
    }
}


// end of file
