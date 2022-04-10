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
            std::map<std::tuple<simplex_t<D> *, bool>, std::shared_ptr<oriented_simplex_t<D>>>;

      public:
        OrientedSimplexFactory() = delete;

        static std::shared_ptr<oriented_simplex_t<D>> & Find(
            const oriented_simplex_t<D> & oriented_simplex)
        {
            // get from the factory the representative of simplices with this composition
            auto & simplex = oriented_simplex.simplex();

            // look up for an oriented simplex with this orientation
            auto ret_find =
                _orientations.find(std::make_tuple(&simplex, oriented_simplex.orientation()));

            // assert the simplex could be found
            assert (ret_find != _orientations.end());

            // return shared pointer to simplex
            return ret_find->second;
        }

        static oriented_simplex_t<D> & OrientedSimplex(simplex_t<D> & simplex, bool orientation)
        {
            // look up for an oriented simplex with this orientation
            auto ret_find = _orientations.find(std::make_tuple(&simplex, orientation));

            // if there was already a simplex with this orientation...
            if (ret_find != _orientations.end()) {
                // report
                // std::cout << "Orientation already present, not adding..." << std::endl;
                // ...return it
                return *ret_find->second;
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
                return *ret_emplace.first->second;

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
                return *ret_emplace.first->second;
            }
        }

        static oriented_simplex_t<D> & OrientedSimplex(
            const simplex_composition_t<D> & composition)
        {
            // get from the factory the representative of simplices with this composition
            auto & simplex = SimplexFactory<D>::Simplex(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return OrientedSimplex(simplex, orientation);
        }

        static void Erase(const oriented_simplex_t<D> & oriented_simplex) requires(D > 0)
        {
#if 0
            if (oriented_simplex->use_count() > 1) return;

            // get footprint of the oriented simplex
            auto & simplex = oriented_simplex->simplex();

            // show me the footprint
            // for (auto & sub_simplex : simplex.simplices()) {
            //     std::cout << sub_simplex << std::endl;
            //     std::cout << sub_simplex.use_count() << std::endl;
            // }

            // get the key to this oriented simplex
            auto mytuple = std::make_tuple(&simplex, oriented_simplex->orientation());

            // erase this oriented simplex from the factory
            _orientations.erase(mytuple);

            // if the footprint is not shared 
            if (oriented_simplex->use_count() == 1) {
                SimplexFactory<D>::Erase(oriented_simplex->footprint());
            }

            // delete oriented simplex
            // TOFIX delete oriented_simplex;
#endif
            // all done
            return;
        }

        static void Erase(oriented_simplex_t<0> * oriented_simplex) //TOFIX: this should be a specialization of the one above
        {
            return;
        }

      private:
        // compute the orientation of the {composition} with respect to the orientation of {simplex}
        static bool _orientation(
            simplex_composition_t<D> composition, const simplex_t<D> & simplex);

      private:
        // container to store the relation (simplex, orientation) -> oriented simplex
        static orientation_map_t _orientations;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        simplex_composition_t<1> composition, const simplex_t<1> & simplex)
    {
        if (composition == simplex.simplices()) {
            return true;
        }
        return false;
    }

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<2>::_orientation(
        simplex_composition_t<2> composition, const simplex_t<2> & simplex)
    {
        auto first_simplex = std::min_element(composition.begin(), composition.end());
        std::rotate(composition.begin(), first_simplex, composition.end());

        auto simplex_cp = simplex.simplices();
        auto first_simplex_cp = std::min_element(simplex_cp.begin(), simplex_cp.end());
        std::rotate(simplex_cp.begin(), first_simplex_cp, simplex_cp.end());

        if (composition == simplex_cp) {
            return true;
        }
        return false;
    }

    template <>    // TODO: implement
    bool OrientedSimplexFactory<3>::_orientation(
        simplex_composition_t<3> composition, const simplex_t<3> & simplex)
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
