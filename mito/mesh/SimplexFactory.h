// code guard
#if !defined(mito_mesh_SimplexFactory_h)
#define mito_mesh_SimplexFactory_h

namespace mito::mesh {
    template <int D>
    class SimplexFactory {

      private:
        // typedef for a composition map of simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // std::map<std::array<simplex_t<0> *, 2>, simplex_t<1> *>  edges composition
        // std::map<std::array<simplex_t<1> *, 3>, simplex_t<2> *>  faces compositions
        // std::map<std::array<simplex_t<2> *, 4>, simplex_t<3> *>  volumes compositions
        using composition_t = std::array<simplex_t<D - 1> *, D + 1>;
        using composition_map_t = std::map<composition_t, simplex_t<D>*>;

      public:
        SimplexFactory() = delete;

        static simplex_t<D> & Simplex(const oriented_simplex_composition_t<D> & composition)
        {
            // pick a representative (factor out equivalence relation)
            auto representative = _representative(composition);

            // if there is no representative registered in the map, then create a new simplex
            // with this composition
            auto ret = _compositions.emplace(representative, new simplex_t<D>(composition));

            // return representative of simplex with composition {composition}
            return *ret.first->second;
        }

        static void Erase(const std::shared_ptr<simplex_t<D>> & simplex)
        {
            // delete the subsimplices
            for (auto & sub_simplex : simplex->simplices()) {
                OrientedSimplexFactory<D - 1>::Erase(sub_simplex);
            }

            // pick a representative (factor out equivalence relation)
            auto representative = _representative(simplex->simplices());

            // erase this simplex from the compositions map
            _compositions.erase(representative);

            // all done
            // (no need to delete simplex because this will be done by shared pointer)
            return;
        }

      private:
        // equivalence class relation for a simplex in 1D
        static composition_t _representative(const oriented_simplex_composition_t<D> & composition);

      private:
        // container to map simplex composition to simplices
        static composition_map_t _compositions;
    };

    // equivalence class relation for a simplex in 1D
    template <>
    SimplexFactory<1>::composition_t SimplexFactory<1>::_representative(
        const oriented_simplex_composition_t<1> & composition)
    {
        composition_t representative { composition[0], composition[1] };
        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());
        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 2D
    template <>
    SimplexFactory<2>::composition_t SimplexFactory<2>::_representative(
        const oriented_simplex_composition_t<2> & composition)
    {
        composition_t representative { &composition[0]->simplex(), &composition[1]->simplex(),
                                       &composition[2]->simplex() };

        // TOFIX: fixes element with 0 orientation
        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());

        // // pick a representative (factor out equivalence relation)
        // auto first_simplex = std::min_element(composition.begin(), composition.end());
        // std::rotate(composition.begin(), first_simplex, composition.end());
        // all done
        return representative;
    }

    template <>    // TODO: implement
    SimplexFactory<3>::composition_t SimplexFactory<3>::_representative(
        const oriented_simplex_composition_t<3> & composition)
    {
        composition_t representative { &composition[0]->simplex(), &composition[1]->simplex(),
                                       &composition[2]->simplex(), &composition[3]->simplex() };

        return representative;
    }

    // initialize static attribute
    template <int D>
    typename SimplexFactory<D>::composition_map_t SimplexFactory<D>::_compositions =
        SimplexFactory<D>::composition_map_t();
}

#endif    // mito_mesh_SimplexFactory_h

// end of file
