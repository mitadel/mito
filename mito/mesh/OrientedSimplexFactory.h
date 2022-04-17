// code guard
#if !defined(mito_mesh_OrientedSimplexFactory_h)
#define mito_mesh_OrientedSimplexFactory_h

namespace mito::mesh {
    template <int D>
    class OrientedSimplexFactory {
      private:
        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and a boolean to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<const simplex_t<D> *, bool>, oriented_simplex_ptr<D>>;

      public:
        OrientedSimplexFactory() = delete;

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

        static oriented_simplex_ptr<D> orientedSimplex(
            const simplex_t<D> & simplex, bool orientation)
        {
            // look up for an oriented simplex with this orientation
            auto ret_find = _orientations.find(std::make_tuple(&simplex, orientation));

            // if there was already a simplex with this orientation...
            if (ret_find != _orientations.end()) {
                // report
                // std::cout << "Orientation already present, not adding..." << std::endl;
                // ...return it
                return ret_find->second;
            }

            // look up for an oriented simplex with opposite orientation
            auto ret_find_opposite = _orientations.find(std::make_tuple(&simplex, !orientation));

            // if there was already a simplex with opposite orientation...
            if (ret_find_opposite != _orientations.end()) {
                // report
                // std::cout << "Found opposite orientation..." << std::endl;
                // ... then insert a new oriented simplex with same footprint but different
                // orientation
                auto mytuple = std::make_tuple(&simplex, orientation);
                auto ret_emplace = _orientations.emplace(
                    mytuple,
                    new oriented_simplex_t<D>(ret_find_opposite->second->footprint(), orientation));
                // report
                // if (ret_emplace.second == false) {
                //     std::cout << "Duplicate orientation: Not inserted..." << std::endl;
                // }

                // return the oriented simplex
                return ret_emplace.first->second;

                // if there was not a simplex with opposite orientation...
            } else {
                // report
                // std::cout << "Opposite orientation not found..." << std::endl;
                // ... then insert a new oriented simplex with same footprint but different
                // orientation
                auto mytuple = std::make_tuple(&simplex, orientation);
                auto ret_emplace = _orientations.emplace(
                    mytuple, new oriented_simplex_t<D>(&simplex, orientation));

                // report
                // if (ret_emplace.second == false) {
                //     std::cout << "Duplicate orientation: Not inserted..." << std::endl;
                // }

                // return the oriented simplex
                return ret_emplace.first->second;
            }
        }

        static oriented_simplex_ptr<D> orientedSimplex(const simplex_composition_t<D> & composition)
        {
            // get from the factory the representative of simplices with this composition
            const auto & simplex = SimplexFactory<D>::simplex(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        static void _cleanup(const oriented_simplex_ptr<D> & oriented_simplex) requires(D > 0)
        {
            // if the oriented simplex is unused 
            if (oriented_simplex->incidence() == 0) {

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

        static void cleanup(const oriented_simplex_ptr<D> & oriented_simplex) requires(D > 0)
        {
            // cleanup recursively until D = 0
            _cleanup(oriented_simplex);

            // all done
            return;
        }

      private:
        // compute the orientation of the {composition} with respect to the orientation of {simplex}
        static bool _orientation(
            const simplex_composition_t<D> & composition, const simplex_t<D> & simplex);

      private:
        // container to store the relation (simplex, orientation) -> oriented simplex
        static orientation_map_t _orientations;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        const simplex_composition_t<1> & composition, const simplex_t<1> & simplex)
    {
        if (composition == simplex.simplices()) {
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
        const simplex_composition_t<2> & composition, const simplex_t<2> & simplex)
    {
        if (_rotate(composition) == _rotate(simplex.simplices())) {
            return true;
        }
        return false;
    }

    template <>    // TODO: implement
    bool OrientedSimplexFactory<3>::_orientation(
        const simplex_composition_t<3> & composition, const simplex_t<3> & simplex)
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
