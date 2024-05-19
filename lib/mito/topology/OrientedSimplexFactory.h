// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once

namespace mito::topology {

    /**
     *
     * This class represents a factory for oriented simplices of order N.
     *
     * The factory class is aware of the class of equivalence for an OrientedSimplex of order N. In
     * fact, being an instance of OrientedSimplex<N> identified by its N+1 subsimplices (through its
     * footprint), there are (N+1)!/2 possible representations of the same oriented simplex. The
     * factory makes sure that at any given time there is at most one OrientedSimplex for an
     * equivalence class, i.e. the representative of the class.
     * The representative of the class of equivalence is chosen by sorting in increasing order the
     * addresses of the instances of the subsimplices.
     */

    template <int N>
    class OrientedSimplexFactory {
      private:
        // typedef for a repository of oriented simplices
        using oriented_simplex_repository_t = utilities::repository_t<simplex_t<N>>;

        // id type of unoriented simplex
        using unoriented_simplex_id_t = utilities::index_t<unoriented_simplex_t<N>>;

        // id type of oriented simplex
        using simplex_id_t = utilities::index_t<simplex_t<N>>;

        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and an orientation to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<unoriented_simplex_id_t, orientation_t>, simplex_id_t>;

      private:
        // default constructor
        OrientedSimplexFactory() :
            _simplex_factory(),
            _oriented_simplices(100 /*segment size */),
            _orientations()
        {}

        // destructor
        ~OrientedSimplexFactory() {}

      private:
        inline auto existsOrientedSimplex(const simplex_composition_t<N> & composition) const
            -> bool
        {
            // find the representative of simplices with composition {composition} from the
            // factory
            auto simplex = _simplex_factory.findSimplex(composition);

            // if the footprint exists, then check if the oriented simplex exists
            if (!simplex.is_nullptr()) {
                // compute the orientation of the current composition with respect to the
                // representative
                orientation_t orientation = _orientation(composition, simplex);

                // check if the simplex exists with the orientation associated with this composition
                return existsOrientedSimplex(simplex, orientation);
            }
            // if the footprint does not exist, then the oriented simplex cannot exist
            else {
                // all done
                return false;
            }
        }

        inline auto existsOrientedSimplex(
            const unoriented_simplex_t<N> & simplex, orientation_t orientation) const -> bool
        {
            // find oriented simplex with the given footprint and orientation
            auto orientedSimplex = findOrientedSimplex(simplex, orientation);

            // if there exists an oriented simplex riding on {simplex} with orientation
            // {orientation}
            return orientedSimplex.is_nullptr() ? false : true;
        }

        inline auto findOrientedSimplex(
            const unoriented_simplex_t<N> & simplex, orientation_t orientation) const
            -> simplex_t<N>
        {
            // bind the footprint and the orientation in a tuple
            auto tuple = std::make_tuple(simplex.id(), orientation);

            // look up the tuple in the orientation map
            auto it_find = _orientations.find(tuple);

            // if there exists an oriented simplex riding on {simplex} with orientation
            // {orientation}
            if (it_find != std::end(_orientations)) {
                // return it
                return _oriented_simplices.resource(it_find->second);
            }
            // if not found
            else {
                // return a shared pointer wrapper around {nullptr}
                return simplex_t<N>();
            }
        }

        // return an oriented simplex riding on footprint {simplex} and with orientation
        // {orientation} (either create a new oriented simplex if such oriented simplex does not
        // exist in the factory or return the existing representative of the class of
        // equivalence of oriented simplices with footprint {simplex} orientation {orientation}
        inline auto orientedSimplex(
            const unoriented_simplex_t<N> & simplex, orientation_t orientation) -> simplex_t<N>
        {
            // bind the footprint and the orientation in a tuple
            auto tuple = std::make_tuple(simplex.id(), orientation);

            // look up the tuple in the orientation map
            auto it_find = _orientations.find(tuple);

            // if an oriented simplex riding on simplex {simplex} with orientation {orientation}
            // is already registered in the map
            if (it_find != std::end(_orientations)) {
                // then return it
                return _oriented_simplices.resource(it_find->second);
            }
            // otherwise
            else {
                // create a new oriented simplex riding on simplex {simplex} with orientation
                // {orientation}
                auto oriented_simplex = _oriented_simplices.emplace(simplex, orientation);

                // register it in the map
                _orientations.insert(std::make_pair(tuple, oriented_simplex.id()));

                // and return it
                return oriented_simplex;
            }
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the
        // class of equivalence of simplices with this composition)
        inline auto orientedSimplex(const simplex_composition_t<N> & composition) -> simplex_t<N>
        requires(N > 0)
        {
            if (!isValid<N>(composition)) {
                pyre::journal::firewall_t firewall("topology::OrientedSimplexFactory");
                firewall << pyre::journal::at(__HERE__) << "Invalid simplex composition."
                         << pyre::journal::endl;
                assert(false);
            }

            // get the representative of simplices with composition {composition} from the factory
            const auto & simplex = _simplex_factory.simplex(composition);

            // compute the orientation of the current composition with respect to the
            // representative
            orientation_t orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        inline auto orientedSimplex(orientation_t orientation) -> simplex_t<0>
        requires(N == 0)
        {
            // get the representative of simplices with composition {composition} from the
            // factory
            const auto & simplex = _simplex_factory.simplex();

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        // instantiate a vertex
        inline auto vertex() -> vertex_t
        requires(N == 0)
        {
            // ask the factory of unoriented vertices for an unoriented vertex
            return _simplex_factory.simplex();
        }

        inline auto simplices() const -> const oriented_simplex_repository_t &
        {
            // return the collection of simplices
            return _oriented_simplices;
        }

        // erase an oriented simplex from the factory (this method actually erases the simplex
        // only if there is no one else using it, otherwise does nothing)
        inline auto erase(simplex_t<N> & oriented_simplex) -> void
        {
            // if this oriented simplex is still being used
            if (oriented_simplex.references() > 1) {
                // do nothing
                return;
            }

            // grab a copy of the footprint
            auto footprint = oriented_simplex->footprint();

            // get footprint of the oriented simplex
            unoriented_simplex_id_t id = oriented_simplex->footprint().id();

            // get the key to this oriented simplex
            auto mytuple = std::make_tuple(id, oriented_simplex->orientation());

            // erase this oriented simplex from the oriented simplex factory
            _orientations.erase(mytuple);

            // erase this oriented simplex from the container
            _oriented_simplices.erase(oriented_simplex);

            // cleanup the unoriented factory around {footprint}
            _simplex_factory.erase(footprint);

            // all done
            return;
        }

      private:
        template <int... J>
        inline auto _permutation_sign(
            const simplex_composition_t<N> & composition,
            const simplex_composition_t<N> & reference, integer_sequence<J...>) const
        {
            // compute the sign of the permutation between the indices of {composition} with respect
            // to the reference composition {reference}
            return mito::math::permutation_sign(
                std::array<simplex_id_t, N + 1>{ composition[J]->footprint().id()... },
                std::array<simplex_id_t, N + 1>{ reference[J]->footprint().id()... });
        }

        // compute the orientation of the {composition} with respect to the reference
        // orientation (that is the orientation of the reference simplex)
        inline auto _orientation(
            const simplex_composition_t<N> & composition,
            const unoriented_simplex_t<N> & simplex) const -> orientation_t
        {
            // compute the permutation sign of {composition} with respect to the composition of
            // {simplex}
            return _permutation_sign(
                composition, simplex->composition(), make_integer_sequence<N + 1>{});
        }

      private:
        // factory for simplices
        simplex_factory_t<N> _simplex_factory;

        // container to store the oriented simplices
        oriented_simplex_repository_t _oriented_simplices;

        // container to store the relation (simplex, orientation) -> oriented simplex
        orientation_map_t _orientations;

        // private friendship with the topology
        friend class Topology;
    };

}


// end of file
