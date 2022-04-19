// code guard
#if !defined(mito_mesh_OrientedSimplexFactory_h)
#define mito_mesh_OrientedSimplexFactory_h

namespace mito::mesh {

    /**
     *
     * This static class represents a factory for oriented simplices of order D.
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
        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and a boolean to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<const simplex_t<D> *, bool>, oriented_simplex_ptr<D>>;

      public:
        // delete default constructor
        OrientedSimplexFactory() = delete;

        // find this oriented simplex in the factory and hand a shared pointer to it (allowing the
        // caller to share its ownership)
        static oriented_simplex_ptr<D> & find(const oriented_simplex_t<D> & oriented_simplex)
        {
            // get from the footprint of this simplex 
            const auto & simplex = oriented_simplex.simplex();

            // look up for an oriented simplex with this orientation
            auto ret_find =
                _orientations.find(std::make_tuple(&simplex, oriented_simplex.orientation()));

            // assert the simplex could be found
            assert (ret_find != _orientations.end());

            // return shared pointer to simplex
            return ret_find->second;
        }

        // return an oriented simplex riding on footprint {simplex} and with orientation
        // {orientation} (either create a new oriented simplex if such oriented simplex does not
        // exist in the factory or return the existing representative of the class of equivalence of
        // oriented simplices with footprint {simplex} orientation {orientation}
        static oriented_simplex_ptr<D> orientedSimplex(
            const simplex_ptr<D> & simplex, bool orientation)
        {

            // bind the footprint and the orientation in a tuple 
            auto mytuple = std::make_tuple(simplex.get(), orientation);

            // if there is no oriented simplex registered in the map riding on simplex {simplex},
            // with orientation {orientation} then create such new oriented simplex and register it
            // in the map
            auto ret =
                _orientations.emplace(mytuple, new oriented_simplex_t<D>(simplex, orientation));

            // return representative of simplex with composition {composition}
            return ret.first->second;
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        static oriented_simplex_ptr<D> orientedSimplex(const simplex_composition_t<D> & composition)
        {
            // get from the factory the representative of simplices with this composition
            auto simplex = SimplexFactory<D>::simplex(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        // returns whether there exists the flipped oriented simplex in the factory
        static bool exists_flipped(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // get the use count of the simplex footprint
            auto use_count = oriented_simplex->footprint().use_count();
            // assert the footprint cannot be used by more than two oriented simplices (on top of
            // the the SimplexFactory)
            assert(use_count == 2 || use_count == 3);
            // return true if the footprint is in used by two oriented simplices
            return use_count == 3 ? true : false;
        }

        // returns the simplex with opposite orientation
        static auto flip(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            return orientedSimplex(oriented_simplex->footprint(), !oriented_simplex->orientation());
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        static int incidence(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            return oriented_simplex.use_count() - 1;
        }

        static void _cleanup(const oriented_simplex_ptr<D> & oriented_simplex) requires(D > 0)
        {
            // if the oriented simplex is unused
            if (incidence(oriented_simplex) == 0) {

                // fetch subsimplices before doing any harm to the oriented simplex
                const auto & subsimplices = oriented_simplex->simplices();

                // get footprint of the oriented simplex
                const auto & simplex = oriented_simplex->simplex();

                // get the key to this oriented simplex
                auto mytuple = std::make_tuple(&simplex, oriented_simplex->orientation());

                // cleanup simplex factory around this oriented simplex
                SimplexFactory<D>::cleanup(oriented_simplex);

                // erase this oriented simplex from the oriented simplex factory
                _orientations.erase(mytuple);

                // erase the subsimplices from the oriented simplex factory
                for (const auto & subsimplex : subsimplices) {
                    OrientedSimplexFactory<D - 1>::_cleanup(subsimplex);
                }
            }

            // all done
            return;
        }

        static void _cleanup(const oriented_simplex_t<D> * oriented_simplex) requires(D == 0)
        {
            // all done (nothing to be done for vertices)
            return;
        }

        // cleanup the factory around an oriented simplex (i.e. remove from the factory unused
        // oriented simplices related to this oriented simplex)
        static void cleanup(const oriented_simplex_ptr<D> & oriented_simplex) requires(D > 0)
        {
            // cleanup recursively until D = 0
            _cleanup(oriented_simplex);

            // all done
            return;
        }

      private:
          // compute the orientation of the {composition} with respect to the orientation of
          // {simplex}
          static bool
          _orientation(
              const simplex_composition_t<D> & composition, const simplex_ptr<D> & simplex);

      private:
        // container to store the relation (simplex, orientation) -> oriented simplex
        static orientation_map_t _orientations;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        const simplex_composition_t<1> & composition, const simplex_ptr<1> & simplex)
    {
        if (composition == simplex->simplices()) {
            return true;
        }
        return false;
    }

    namespace {
        auto _rotate(const simplex_composition_t<2> & composition)
        {
            // an array of pointers to oriented simplices
            using oriented_simplex_array_t = std::array<const oriented_simplex_t<1> *, 3>;

            // get the oriented simplices from the shared pointers
            auto composition_copy =
                oriented_simplex_array_t { composition[0].get(), composition[1].get(),
                                           composition[2].get() };
            auto first_simplex = std::min_element(composition_copy.begin(), composition_copy.end());
            std::rotate(composition_copy.begin(), first_simplex, composition_copy.end());

            // return rotated composition
            return composition_copy;
        }
    }

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<2>::_orientation(
        const simplex_composition_t<2> & composition, const simplex_ptr<2> & simplex)
    {
        if (_rotate(composition) == _rotate(simplex->simplices())) {
            return true;
        }
        return false;
    }

    template <>    // TODO: implement
    bool OrientedSimplexFactory<3>::_orientation(
        const simplex_composition_t<3> & composition, const simplex_ptr<3> & simplex)
    {
        return true;
    }

    // initialize static attribute
    template <int D>
    typename OrientedSimplexFactory<D>::orientation_map_t OrientedSimplexFactory<D>::_orientations =
        OrientedSimplexFactory<D>::orientation_map_t();
}

#endif    // mito_mesh_OrientedSimplexFactory_h

// end of file
