// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_topology_FlipDiagonal_h)
#define mito_topology_FlipDiagonal_h


namespace mito::topology {

    class FlipDiagonal {

      private:
        // typedef simplex type
        using simplex_type = simplex_t<2>;
        // typedef a pair of simplices
        using simplex_pair_type = std::pair<simplex_type, simplex_type>;
        // typedef subsimplex type
        using subsimplex_type = simplex_t<1>;
        // typedef unoriented subsimplex type
        using unoriented_subsimplex_type = unoriented_simplex_t<1>;
        // typedef simplex composition type
        using simplex_composition_type = simplex_composition_t<2>;
        // typedef a set of vertices
        // need a regular set (not an unordered one) because set_difference works with ordered sets
        using vertex_set_type = std::set<vertex_t>;
        // typedef a set of subsimplices
        using subsimplex_set_type = std::set<subsimplex_type>;

      private:
        static inline auto _find_shared_simplex(
            const simplex_type & simplex0, const simplex_type & simplex1)
            -> const unoriented_subsimplex_type &;
        static inline auto _opposite_vertices(
            const simplex_type & simplex0, const simplex_type & simplex1,
            const auto & shared_simplex) -> auto;
        static inline auto _erase_subsimplex(
            subsimplex_set_type & boundary_simplices,
            simplex_composition_type & new_simplex_composition,
            subsimplex_type & subsimplex_to_erase, size_t i) -> void;

      public:
        // flip the diagonal between one pair of adjacent simplices
        static inline auto flip_diagonal(const simplex_pair_type & simplex_pair)
            -> simplex_pair_type;
    };

    auto flip_diagonal(const std::pair<simplex_t<2>, simplex_t<2>> & simplex_pair)
        -> std::pair<simplex_t<2>, simplex_t<2>>
    {
        return FlipDiagonal::flip_diagonal(simplex_pair);
    }
}


// get the inline definitions
#define mito_topology_FlipDiagonal_icc
#include "FlipDiagonal.icc"
#undef mito_topology_FlipDiagonal_icc


#endif

// end of file
