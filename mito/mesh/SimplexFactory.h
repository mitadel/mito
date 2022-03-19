// code guard
#if !defined(mito_mesh_SimplexFactory_h)
#define mito_mesh_SimplexFactory_h

namespace mito::mesh {
    template <int D>
    class SimplexFactory {
      public:
        // typedef for simplex composition
        using simplex_composition_t = Simplex<D>::simplex_composition_t;

      private:
        // typedef for a composition map of simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // std::map<std::array<simplex_t<0> *, 2>, simplex_t<1> *>  edges composition
        // std::map<std::array<simplex_t<1> *, 3>, simplex_t<2> *>  faces compositions
        // std::map<std::array<simplex_t<2> *, 4>, simplex_t<3> *>  volumes compositions
        using composition_map_t = std::map<simplex_composition_t, simplex_t<D> *>;

      public:
        SimplexFactory() = delete;

        static simplex_t<D> * Simplex(const simplex_composition_t & composition)
        {
            // pick a representative (factor out equivalence relation)
            auto representative = composition;
            _representative(representative);

            // if there is no representative registered in the map, then create a new simplex 
            // with this composition
            auto ret = _compositions.emplace(representative, new simplex_t<D>(composition));

            // return representative of simplex with composition {composition} 
            return ret.first->second;
        }

      private:
        // equivalence class relation for a simplex in 1D
        static void _representative(simplex_composition_t & composition);

      private:
        // container to map simplex composition to simplices
        static composition_map_t _compositions;
    };

    // equivalence class relation for a simplex in 1D
    template <>
    void SimplexFactory<1>::_representative(SimplexFactory<1>::simplex_composition_t & composition)
    {
        // pick a representative (factor out equivalence relation)
        std::sort(composition.begin(), composition.end());
        // all done
        return;
    }

    // equivalence class relation for a simplex in 2D
    template <>
    void SimplexFactory<2>::_representative(SimplexFactory<2>::simplex_composition_t & composition)
    {
        // pick a representative (factor out equivalence relation)
        auto first_simplex = std::min_element(composition.begin(), composition.end());
        std::rotate(composition.begin(), first_simplex, composition.end());
        // all done
        return;
    }

    // initialize static attribute
    template <int D>
    SimplexFactory<D>::composition_map_t SimplexFactory<D>::_compositions =
        SimplexFactory<D>::composition_map_t();
}

#endif    // mito_mesh_SimplexFactory_h

// end of file
