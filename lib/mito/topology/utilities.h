// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::topology {

    // number of vertices of simplex
    template <class cellT>
    constexpr auto n_vertices() -> int
    {
        return cellT::resource_type::n_vertices;
    }

    // cell family of simplex
    template <class cellT, int I>
    using cell_family = typename cellT::resource_type::template cell_family_type<I>;

    // overload operator<< for oriented simplices
    template <int N>
    std::ostream & operator<<(std::ostream & os, const simplex_t<N> & s)
    {
        // print orientation
        os << "orientation: " << s->orientation() << std::endl;
        // print footprint
        os << "footprint: " << s->footprint() << std::endl;
        // all done
        return os;
    }

    // overload operator<< for simplices
    template <int N>
    std::ostream & operator<<(std::ostream & os, const unoriented_simplex_t<N> & s)
    {
        os << s.id() << " composed of:" << std::endl;
        for (const auto & simplex : s->composition()) {
            os << "\t" << simplex << std::endl;
        }
        return os;
    }

    // overload operator<< for oriented simplices
    template <>
    std::ostream & operator<<(std::ostream & os, const unoriented_simplex_t<0> & s)
    {
        os << "vertex: " << s.id() << std::endl;
        return os;
    }

    // TOFIX: decide on one way to print simplices nicely, support that and remove the rest
    // overload operator<< for oriented simplices
    template <int N>
    auto print(const simplex_t<N> & s) -> void
    {
        // make a channel
        journal::info_t channel("lib.topology.simplex");

        // print footprint and orientation
        channel << "footprint: " << s->footprint().id() << ", orientation: " << s->orientation()
                << journal::endl;
        // print footprint
        print(s->footprint());
        // all done
        return;
    }

    // overload operator<< for simplices
    template <int N>
    auto print(const unoriented_simplex_t<N> & s) -> void
    {
        // make a channel
        journal::info_t channel("lib.topology.simplex");

        if constexpr (N == 0)
            channel << "vertex ";

        if constexpr (N == 1)
            channel << "segment ";

        if constexpr (N == 2)
            channel << "triangle ";

        if constexpr (N == 3)
            channel << "tetrahedron ";

        channel << s.id() << " composed of:" << journal::endl;
        channel << journal::indent(1) << journal::endl;
        for (const auto & simplex : s->composition()) {
            // channel << simplex.id() << journal::endl;
            print(simplex);
        }
        return;
    }

    // overload operator<< for oriented simplices
    template <>
    auto print(const unoriented_simplex_t<0> & s) -> void
    {
        // make a channel
        journal::info_t channel("lib.topology.simplex");
        channel << journal::indent(1) << "vertex: " << s.id() << journal::endl;

        return;
    }

    auto tail(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        return oriented_simplex->composition()[0]->footprint();
    }

    auto head(const simplex_t<1> & oriented_simplex) -> vertex_t
    {
        return oriented_simplex->composition()[1]->footprint();
    }

    bool headTailConnected(const simplex_t<1> & simplex_1, const simplex_t<1> & simplex_2)
    {
        if (head(simplex_1) == tail(simplex_2)) {
            return true;
        }

        return false;
    }

    // checks whether a given simplex composition is valid, i.e. if all simplices in {composition}
    // have consistent orientations with each other
    // NOTE: the result of this function should not depend on the order of the simplices in
    //          {composition}
    // TODO: add a test to check this
    template <int N>
    auto isValid(const simplex_composition_t<N> & composition) -> bool
    requires(N > 1)
    {
        // id type of unoriented simplex
        using cell_id_type = utilities::index_t<unoriented_simplex_t<N>>;
        // map from a simplex id to an integer
        using orientation_map_type = std::unordered_map<cell_id_type, int>;
        // map to store the net orientation (i.e. positive-orientation occurrences minus
        // negative-orientation occurrences) of a given unoriented simplex
        // this map is used to determine whether the simplices in composition have consistent
        // orientation with each other (i.e. if its (N-2)-subsimplices appear two
        // times with two opposite orientations
        orientation_map_type orientation_map;

        // loop on all the (N-1)-simplices in {composition}
        for (const auto & simplex : composition) {
            // loop on all the (N-2)-simplices in {composition}
            for (const auto & subsimplex : simplex->composition()) {
                // populate the orientation map with the {subsimplex} id and its 'net' orientation
                orientation_map[subsimplex->footprint().id()] +=
                    subsimplex->orientation() * simplex->orientation();
            }
        }

        // assert that the {orientation_map} has as many entries as the number of (unoriented) edges
        // in an N-simplex
        assert(std::size(orientation_map) == ((N + 1) * N) / 2);

        // check that the net orientation of each (N-2)-subsimplex is 0
        // (i.e. the subsimplex appears twice, with each opposite orientations)
        for (const auto & item : orientation_map) {
            // if any of the simplices in {orientation_map} has not zero net orientation within the
            // {composition}...
            if (item.second != 0) {
                // {composition} is not a valid simplex composition
                return false;
            }
        }

        // otherwise, {composition} is a valid simplex composition
        return true;
    }

    template <int N>
    auto isValid(const simplex_composition_t<N> & composition) -> bool
    requires(N == 1)
    {
        // a segment is not valid if the two vertices coincide
        if (composition[0]->footprint() == composition[1]->footprint()) {
            return false;
        }

        // a segment is not valid if the two vertices have same orientation
        if (composition[0]->orientation() == composition[1]->orientation()) {
            return false;
        }

        return true;
    }

    // returns the topological simplex with opposite orientation to {simplex}
    template <int N>
    constexpr auto flip(const simplex_t<N> & simplex) -> simplex_t<N>
    {
        return topology().flip(simplex);
    }
}


// end of file
