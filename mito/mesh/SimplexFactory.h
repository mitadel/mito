// code guard
#if !defined(mito_mesh_SimplexFactory_h)
#define mito_mesh_SimplexFactory_h

namespace mito::mesh {

    /**
     *
     * This static class represents a factory for simplices of order D.
     *
     * The factory class is aware of the class of equivalence for a Simplex of order D. In fact,
     * being an instance of Simplex<D> identified by its D+1 subsimplices, there are (D+1)! possible
     * representations of the same simplex. The factory makes sure that at any given time
     * there is at most one simplex for an equivalence class, i.e. the representative of the class.
     * The representative of the class of equivalence is chosen by sorting in increasing order the
     * addresses of the instances of the subsimplices.
     */

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
        using composition_t = std::array<const simplex_t<D - 1> *, D + 1>;
        using composition_map_t = std::map<composition_t, simplex_ptr<D>>;

      public:
        SimplexFactory() = delete;

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        static simplex_ptr<D> simplex(const simplex_composition_t<D> & composition)
        {
            // pick a representative (factor out equivalence relation)
            auto representative = _representative(composition);

            // if there is no representative registered in the map, then create a new simplex
            // with this composition
            // TODO: placement new here
            auto ret = _compositions.emplace(representative, new simplex_t<D>(composition));

            // return representative of simplex with composition {composition}
            return ret.first->second;
        }

        // cleanup the factory around an oriented simplex (i.e. remove the simplex footprint from 
        // the factory if this oriented simplex is the only owner of its footprint)
        static void cleanup(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // if the footprint is not shared
            if (!oriented_simplex->exists_flipped()) {

                // pick a representative (factor out equivalence relation)
                auto representative = _representative(oriented_simplex->simplices());

                // erase this simplex from the compositions map
                _compositions.erase(representative);
            }

            // all done
            return;
        }

      private:
        // equivalence class relation for a simplex
        static auto _representative(const simplex_composition_t<D> & composition);

      private:
        // container to map simplex composition to simplices
        static composition_map_t _compositions;
    };

    // equivalence class relation for a simplex in 1D
    template <>
    auto SimplexFactory<1>::_representative(const simplex_composition_t<1> & composition)
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0], composition[1] };
        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());
        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 2D
    template <>
    auto SimplexFactory<2>::_representative(const simplex_composition_t<2> & composition)
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { &composition[0]->simplex(), &composition[1]->simplex(),
                                       &composition[2]->simplex() };

        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());

        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 3D
    template <>
    auto SimplexFactory<3>::_representative(const simplex_composition_t<3> & composition)
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { &composition[0]->simplex(), &composition[1]->simplex(),
                                       &composition[2]->simplex(), &composition[3]->simplex() };

        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());

        // all done
        return representative;
    }

    // initialize static attribute
    template <int D>
    typename SimplexFactory<D>::composition_map_t SimplexFactory<D>::_compositions =
        SimplexFactory<D>::composition_map_t();
}

#endif    // mito_mesh_SimplexFactory_h

// end of file
