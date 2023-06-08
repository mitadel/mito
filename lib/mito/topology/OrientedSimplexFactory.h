// code guard
#if !defined(mito_topology_OrientedSimplexFactory_h)
#define mito_topology_OrientedSimplexFactory_h

namespace mito::topology {

    /**
     *
     * This class represents a factory for oriented simplices of order D.
     *
     * The factory class is aware of the class of equivalence for an OrientedSimplex of order D. In
     * fact, being an instance of OrientedSimplex<D> identified by its D+1 subsimplices (through its
     * footprint), there are (D+1)!/2 possible representations of the same oriented simplex. The
     * factory makes sure that at any given time there is at most one OrientedSimplex for an
     * equivalence class, i.e. the representative of the class. The representative of the class of
     * equivalence is chosen by starting the composition with the subsimplex of smallest address and
     * by circularly rotating the other simplices in the composition array around the first simplex.
     */

    template <int D>
    class OrientedSimplexFactory {
      private:
        // typedef for a collection of oriented simplices
        using oriented_simplex_collection_t = utilities::segmented_t<simplex_t<D>>;

        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and a boolean to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<unoriented_simplex_id_t<D>, bool>, simplex_t<D>>;

      public:    // TOFIX: should be private but the default constructor of tuple needs it public
        // default constructor
        OrientedSimplexFactory() :
            _simplex_factory(),
            _oriented_simplices(100 /*segment size */),
            _orientations() {};

        // destructor
        ~OrientedSimplexFactory()
        {
            if (_oriented_simplices.size() > 0) {
                for (const auto & oriented_simplex : _oriented_simplices) {
                    oriented_simplex->~OrientedSimplex<D>();
                }
            }
        };

      private:
        inline auto existsOrientedSimplex(const simplex_composition_t<D> & composition) const
            -> bool
        {
            // find the representative of simplices with composition {composition} from the
            // factory
            auto simplex = _simplex_factory.findSimplex(composition);

            // if the footprint exists, then check if the oriented simplex exists
            if (simplex) {
                // compute the orientation of the current composition with respect to the
                // representative
                bool orientation = _orientation(composition, *simplex);

                // check if the simplex exists with the orientation associated with this composition
                return existsOrientedSimplex(*simplex, orientation);
            }
            // if the footprint does not exist, then the oriented simplex cannot exist
            else {
                // all done
                return false;
            }
        }

        inline auto existsOrientedSimplex(
            const unoriented_simplex_t<D> & simplex, bool orientation) const -> bool
        {
            // find oriented simplex with the given footprint and orientation
            auto orientedSimplex = findOrientedSimplex(simplex, orientation);

            // if there exists an oriented simplex riding on {simplex} with orientation
            // {orientation}
            return orientedSimplex ? true : false;
        }

        inline auto findOrientedSimplex(const simplex_composition_t<D> & composition) const
            -> const simplex_t<D> *
        {
            // find the representative of simplices with composition {composition} from the
            // factory
            auto simplex = _simplex_factory.findSimplex(composition);

            // if the footprint is found, then find the oriented simplex that uses it
            if (simplex) {
                // compute the orientation of the current composition with respect to the
                // representative
                bool orientation = _orientation(composition, *simplex);

                // find oriented simplex with the given footprint and orientation
                return findOrientedSimplex(*simplex, orientation);
            }
            // if the footprint is not found, then there is no oriented simplex
            else {
                // all done
                return nullptr;
            }
        }

        inline auto findOrientedSimplex(
            const unoriented_simplex_t<D> & simplex, bool orientation) const -> const simplex_t<D> *
        {
            // bind the footprint and the orientation in a tuple
            auto tuple = std::make_tuple(simplex.id(), orientation);

            // look up the tuple in the orientation map
            auto it_find = _orientations.find(tuple);

            // if there exists an oriented simplex riding on {simplex} with orientation
            // {orientation}
            if (it_find != _orientations.end()) {
                // return it
                return &(it_find->second);
            }
            // if not found
            else {
                // all done
                return nullptr;
            }
        }

        // return an oriented simplex riding on footprint {simplex} and with orientation
        // {orientation} (either create a new oriented simplex if such oriented simplex does not
        // exist in the factory or return the existing representative of the class of
        // equivalence of oriented simplices with footprint {simplex} orientation {orientation}
        inline auto orientedSimplex(const unoriented_simplex_t<D> & simplex, bool orientation)
            -> const simplex_t<D> &
        {
            // find oriented simplex with the given footprint and orientation
            auto orientedSimplex = findOrientedSimplex(simplex, orientation);

            // if an oriented simplex riding on simplex {simplex} with orientation {orientation}
            // is already registered in the map
            if (orientedSimplex) {
                // then return it
                return *orientedSimplex;
            }
            // otherwise
            else {
                // bind the footprint and the orientation in a tuple
                auto tuple = std::make_tuple(simplex.id(), orientation);

                // create a new oriented simplex riding on simplex {simplex} with orientation
                // {orientation} and register it in the map
                auto it = _orientations.insert(
                    std::make_pair(tuple, _emplace_simplex(simplex, orientation)));

                // and return it
                return it.first->second;
            }
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the
        // class of equivalence of simplices with this composition)
        inline auto orientedSimplex(const simplex_composition_t<D> & composition)
            -> const simplex_t<D> &
        requires(D > 0)
        {
            if (!isValid(composition)) {
                pyre::journal::firewall_t firewall("topology::OrientedSimplexFactory");
                firewall << pyre::journal::at(__HERE__)
                         << "I cannot create an OrientedSimplex from a simplex composition "
                         << "that is not head-tail connected." << pyre::journal::endl;
                assert(false);
            }

            // get the representative of simplices with composition {composition} from the
            // factory
            const auto & simplex = _simplex_factory.simplex(composition);

            // compute the orientation of the current composition with respect to the
            // representative
            bool orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        inline auto orientedSimplex(bool orientation) -> const simplex_t<0> &
        requires(D == 0)
        {
            // get the representative of simplices with composition {composition} from the
            // factory
            const auto & simplex = _simplex_factory.simplex();

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        // instantiate a vertex
        inline auto vertex() -> const vertex_t &
        requires(D == 0)
        {
            // ask the factory of unoriented vertices for an unoriented vertex
            return _simplex_factory.simplex();
        }

        // erase an oriented simplex from the factory (this method actually erases the simplex
        // only if there is no one else using it, otherwise does nothing)
        inline auto erase(const simplex_t<D> & oriented_simplex) -> void
        {
            // sanity check
            assert(oriented_simplex.references() > 0);

            // grab a copy of the footprint
            auto footprint = oriented_simplex->footprint();

            // get footprint of the oriented simplex
            unoriented_simplex_id_t<D> id = oriented_simplex->footprint().id();

            // get the key to this oriented simplex
            auto mytuple = std::make_tuple(id, oriented_simplex->orientation());

            // erase the simplex
            oriented_simplex->_erase();

            // erase this oriented simplex from the oriented simplex factory
            _orientations.erase(mytuple);

            // if this simplex is the last one using the footprint (other than the copy we just
            // did)
            if (footprint.references() == 2) {
                // cleanup the unoriented factory around {footprint}
                _simplex_factory.erase(footprint);
            }

            // all done
            return;
        }

      private:
        // compute the orientation of the {composition} with respect to the orientation of
        // {simplex}
        inline bool _orientation(
            const simplex_composition_t<D> & composition,
            const unoriented_simplex_t<D> & simplex) const;

        inline auto _emplace_simplex(const unoriented_simplex_t<D> & simplex, bool orientation)
            -> simplex_t<D>
        {
            // get from {_simplices} the location where to place the new simplex
            auto location = _oriented_simplices.location_for_placement();

            // create a new simplex at location {location} with placement new
            OrientedSimplex<D> * resource = new (location) OrientedSimplex<D>(simplex, orientation);

            // wrap the new simplex in a shared pointer and return it
            return utilities::shared_ptr<OrientedSimplex<D>>(resource, &_oriented_simplices);
        }

      private:
        // factory for simplices
        simplex_factory_t<D> _simplex_factory;

        // container to store the oriented simplices
        oriented_simplex_collection_t _oriented_simplices;

        // container to store the relation (simplex, orientation) -> oriented simplex
        orientation_map_t _orientations;

        // private friendship with the topology
        friend class Topology;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        const simplex_composition_t<1> & composition, const unoriented_simplex_t<1> & simplex) const
    {
        if (composition == simplex->composition()) {
            return true;
        }
        return false;
    }

    namespace {
        inline auto _rotate(const simplex_composition_t<2> & composition)
        {
            // an array of oriented simplices ids
            using oriented_simplex_array_t = std::array<oriented_simplex_id_t<2>, 3>;

            // get the oriented simplices from the shared pointers
            auto composition_copy =
                oriented_simplex_array_t { composition[0].id(), composition[1].id(),
                                           composition[2].id() };
            auto first_simplex = std::min_element(composition_copy.begin(), composition_copy.end());
            std::rotate(composition_copy.begin(), first_simplex, composition_copy.end());

            // return rotated composition
            return composition_copy;
        }
    }

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<2>::_orientation(
        const simplex_composition_t<2> & composition, const unoriented_simplex_t<2> & simplex) const
    {
        if (_rotate(composition) == _rotate(simplex->composition())) {
            return true;
        }
        return false;
    }

    template <>    // TODO: implement
    bool OrientedSimplexFactory<3>::_orientation(
        const simplex_composition_t<3> & /*composition*/,
        const unoriented_simplex_t<3> & /*simplex*/) const
    {
        return true;
    }
}

#endif    // mito_topology_OrientedSimplexFactory_h

// end of file
