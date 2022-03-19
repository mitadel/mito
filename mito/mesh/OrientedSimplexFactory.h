// code guard
#if !defined(mito_mesh_OrientedSimplexFactory_h)
#define mito_mesh_OrientedSimplexFactory_h

namespace mito::mesh {
    // TODO: forbid user to create simplices without passing through the factory
    template <int D>    // TOFIX: not sure we need this template parameter
    class OrientedSimplexFactory {
      private:
        // typedef for simplex composition
        template <int I>
        using simplex_composition_t = Simplex<I>::simplex_composition_t;

        // typedef for an orientation map of simplices:
        // these maps map:
        //      a simplex pointer and a boolean to an oriented simplex pointer 
        template <int I>
        using orientation_map = std::map<std::tuple<simplex_t<I> *, bool>, 
            oriented_simplex_t<I> *>;

        template <typename = std::make_index_sequence<D>>
        struct orientation_tuple_t;

        template <size_t... I>
        struct orientation_tuple_t<std::index_sequence<I...>> {
            using type = std::tuple<orientation_map<int(I + 1)>...>;
        };

        // this expands to:
        // tuple<orientation_map<1>, ..., orientation_map<D>
        using orientation_map_t = typename orientation_tuple_t<>::type;

      public:
        OrientedSimplexFactory() = delete;

        template <int I>
        static oriented_simplex_t<I> * OrientedSimplex(const simplex_composition_t<I> & composition)
        {
            // get from the factory the representative of simplices with this composition
            auto * simplex = mito::mesh::SimplexFactory<D>::template Simplex<I>(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, *simplex);

            // look up for an oriented simplex with opposite orientation
            auto ret_find =
                std::get<I - 1>(_orientations).find(std::make_tuple(simplex, !orientation));

            // if there was already a simplex with opposite orientation... 
            if (ret_find != std::get<I - 1>(_orientations).end()) {
                // report
                std::cout << "Found opposite orientation..." << std::endl;
                // ... then insert a new oriented simplex with same footprint but different
                // orientation
                auto mytuple = std::make_tuple(simplex, orientation);
                auto ret_emplace =
                    std::get<I - 1>(_orientations)
                        .emplace(
                            mytuple,
                            new oriented_simplex_t<I>(ret_find->second->footprint(), orientation));
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
                    std::get<I - 1>(_orientations)
                        .emplace(mytuple, new oriented_simplex_t<I>(simplex, orientation));

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
        static bool _orientation(
            const simplex_composition_t<1> & composition, const simplex_t<1> & simplex)
        {
            if (composition == simplex.simplices()) {
                return true;
            }
            return false;
        }

        // compute the orientation of the {composition} with respect to the orientation of {simplex}
        static bool _orientation(
            const simplex_composition_t<2> & composition, const simplex_t<2> & simplex)
        {
            auto first_simplex = std::min_element(composition.begin(), composition.end());
            std::rotate(composition.begin(), first_simplex, composition.end());
            if (composition == simplex.simplices()) {
                return true;
            }
            return false;
        }

      private:
        // container to ... //TOFIX
        static orientation_map_t _orientations;
    };

    // initialize static attribute
    template <int D>
    mito::mesh::OrientedSimplexFactory<D>::orientation_map_t
        mito::mesh::OrientedSimplexFactory<D>::_orientations =
            mito::mesh::OrientedSimplexFactory<D>::orientation_map_t();
}

#endif    // mito_mesh_OrientedSimplexFactory_h

// end of file
