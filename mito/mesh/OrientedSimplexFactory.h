// code guard
#if !defined(mito_mesh_OrientedSimplexFactory_h)
#define mito_mesh_OrientedSimplexFactory_h

namespace mito::mesh {
    template <int D>
    class OrientedSimplexFactory {
      private:
        // typedef for simplex composition
        using simplex_composition_t = typename Simplex<D>::simplex_composition_t;

        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and a boolean to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<simplex_t<D> *, bool>, oriented_simplex_t<D> *>;

      public:
        OrientedSimplexFactory() = delete;

        static oriented_simplex_t<D> * OrientedSimplex(const simplex_composition_t & composition)
        {
            // get from the factory the representative of simplices with this composition
            auto * simplex = SimplexFactory<D>::Simplex(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, *simplex);

            // look up for an oriented simplex with opposite orientation
            auto ret_find = _orientations.find(std::make_tuple(simplex, !orientation));

            // if there was already a simplex with opposite orientation... 
            if (ret_find != _orientations.end()) {
                // report
                std::cout << "Found opposite orientation..." << std::endl;
                // ... then insert a new oriented simplex with same footprint but different
                // orientation
                auto mytuple = std::make_tuple(simplex, orientation);
                auto ret_emplace = _orientations.emplace(
                    mytuple, new oriented_simplex_t<D>(ret_find->second->footprint(), orientation));
                // report
                if (ret_emplace.second == false) {
                    std::cout << "Duplicate orientation: Not inserted..." << std::endl;
                }

                // return the oriented simplex
                return ret_emplace.first->second;

            // if there was not a simplex with opposite orientation...
            } else {
                // report
                std::cout << "Opposite orientation not found..." << std::endl;
                // ... then insert a new oriented simplex with same footprint but different
                // orientation
                auto mytuple = std::make_tuple(simplex, orientation);
                auto ret_emplace =
                    _orientations.emplace(mytuple, new oriented_simplex_t<D>(simplex, orientation));

                // report
                if (ret_emplace.second == false) {
                    std::cout << "Duplicate orientation: Not inserted..." << std::endl;
                }

                // return the oriented simplex
                return ret_emplace.first->second;
            }
        }

      private:
        // compute the orientation of the {composition} with respect to the orientation of {simplex}
        static bool _orientation(simplex_composition_t composition, const simplex_t<D> & simplex);

      private:
        // container to store the relation (simplex, orientation) -> oriented simplex
        static orientation_map_t _orientations;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        OrientedSimplexFactory<1>::simplex_composition_t composition, const simplex_t<1> & simplex)
    {
        if (composition == simplex.simplices()) {
            return true;
        }
        return false;
    }

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<2>::_orientation(
        OrientedSimplexFactory<2>::simplex_composition_t composition, const simplex_t<2> & simplex)
    {
        auto first_simplex = std::min_element(composition.begin(), composition.end());
        std::rotate(composition.begin(), first_simplex, composition.end());
        if (composition == simplex.simplices()) {
            return true;
        }
        return false;
    }

    // initialize static attribute
    template <int D>
    typename OrientedSimplexFactory<D>::orientation_map_t OrientedSimplexFactory<D>::_orientations =
        OrientedSimplexFactory<D>::orientation_map_t();
}

#endif    // mito_mesh_OrientedSimplexFactory_h

// end of file
