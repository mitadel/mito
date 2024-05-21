// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::topology {

    /**
     *
     *
     */

    class Topology {
      private:
        // default constructor
        Topology();

        // delete copy constructor
        Topology(const Topology &) = delete;

        // delete assignment operator
        void operator=(const Topology &) = delete;

        // destructor
        ~Topology();

      public:
        template <int N>
        inline auto simplex(const unoriented_simplex_t<N> & footprint, orientation_t orientation)
            -> simplex_t<N>;

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        template <int N>
        inline auto simplex(const simplex_composition_t<N> & composition) -> simplex_t<N>
        requires(N > 0);

        template <int N>
        inline auto simplex(orientation_t orientation) -> simplex_t<0>
        requires(N == 0);

        // return a simplex with vertices {vertices} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the
        // class of equivalence of simplices with this composition)
        template <int N>
        inline auto simplex(const vertex_simplex_composition_t<N> & vertices) -> simplex_t<N>
        requires(N >= 1 && N <= 3);

        // instantiate a vertex
        inline auto vertex() -> vertex_t;

        // instantiate a segment
        inline auto segment(const simplex_composition_t<1> & simplices) -> simplex_t<1>;

        // instantiate a triangle
        inline auto triangle(const simplex_composition_t<2> & simplices) -> simplex_t<2>;

        // instantiate a tetrahedron
        inline auto tetrahedron(const simplex_composition_t<3> & simplices) -> simplex_t<3>;

        // instantiate a segment from unoriented vertices
        inline auto segment(const vertex_simplex_composition_t<1> & simplices) -> simplex_t<1>;

        // instantiate a triangle
        inline auto triangle(const vertex_simplex_composition_t<2> & vertices) -> simplex_t<2>;

        // instantiate a tetrahedron
        inline auto tetrahedron(const vertex_simplex_composition_t<3> & vertices) -> simplex_t<3>;

      private:
        template <int N>
        inline auto _erase(simplex_t<N> & simplex) -> void
        requires(N == 0);

        template <int N>
        inline auto _erase(simplex_t<N> & simplex) -> void
        requires(N > 0);

        // mutator for the simplex factory of dimension N
        template <int N>
        inline auto _get_factory() noexcept -> oriented_simplex_factory_t<N> &;

        // accessor for the simplex factory of dimension N
        template <int N>
        inline auto _get_factory() const noexcept -> const oriented_simplex_factory_t<N> &;

      public:
        // returns the number of simplices of dimension {N}
        template <int N>
        inline auto n_simplices() const -> int;

        // returns whether the oriented simplex exists in the factory
        template <int N>
        inline auto exists(const simplex_composition_t<N> & simplices) const -> bool;

        // returns whether the segment exists in the factory
        inline auto exists(const vertex_simplex_composition_t<1> & vertices) const -> bool;

        // returns whether there exists the flipped oriented simplex in the factory
        template <int N>
        inline auto exists_flipped(const simplex_t<N> & simplex) const -> bool;

        // returns the simplex with opposite orientation
        template <int N>
        inline auto flip(const simplex_t<N> & simplex) -> simplex_t<N>;

        template <int N>
        inline auto erase(simplex_t<N> & simplex) -> void;

      private:
        // factory for vertices
        oriented_simplex_factory_t<0> _vertex_factory;

        // factory for segments
        oriented_simplex_factory_t<1> _segment_factory;

        // factory for triangles
        oriented_simplex_factory_t<2> _triangle_factory;

        // factory for tetrahedra
        oriented_simplex_factory_t<3> _tetrahedron_factory;

        // friendship with the singleton
        using TopologySingleton = utilities::Singleton<Topology>;
        friend TopologySingleton;
    };
}


// get the inline definitions
#define mito_topology_Topology_icc
#include "Topology.icc"
#undef mito_topology_Topology_icc


// end of file
