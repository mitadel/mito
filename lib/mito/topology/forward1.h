// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::topology {

    // class simplex
    template <int N>
    class Simplex;

    // class oriented simplex
    template <int N>
    class OrientedSimplex;

    // class simplex factory
    template <int N>
    class SimplexFactory;

    // simplex factory alias
    template <int N>
    using simplex_factory_t = SimplexFactory<N>;

    // class oriented simplex factory
    template <int N>
    class OrientedSimplexFactory;

    // oriented simplex factory alias
    template <int N>
    using oriented_simplex_factory_t = OrientedSimplexFactory<N>;

    // class topology
    class Topology;

    // topology alias
    using topology_t = Topology;

    // unoriented simplex alias
    template <int N>
    using unoriented_simplex_t = utilities::shared_ptr<const Simplex<N>>;

    // oriented simplex alias
    template <int N>
    using oriented_simplex_t = utilities::shared_ptr<const OrientedSimplex<N>>;

    // type for simplex orientation
    using orientation_t = int;
}


// end of file
