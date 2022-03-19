// code guard
#if !defined(mito_mesh_SimplexFactory_h)
#define mito_mesh_SimplexFactory_h

namespace mito::mesh {
    // TODO: forbid user to create simplices without passing through the factory
    template <int D>    // TOFIX: not sure we need this template parameter
    class SimplexFactory {
      public:
        // typedef for simplex composition
        template <int I>
        using simplex_composition_t = Simplex<I>::simplex_composition_t;

      private:
        // typedef for a composition map of simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // std::map<std::array<simplex_t<0> *, 2>, simplex_t<1> *>  edges composition
        // std::map<std::array<simplex_t<1> *, 3>, simplex_t<2> *>  faces compositions
        // std::map<std::array<simplex_t<2> *, 4>, simplex_t<3> *>  volumes compositions
        template <int I>
        using composition_map_t = std::map<simplex_composition_t<I>, simplex_t<I> *>;

        template <typename = std::make_index_sequence<D>>
        struct composition_tuple;

        template <size_t... I>
        struct composition_tuple<std::index_sequence<I...>> {
            using type = std::tuple<composition_map_t<int(I + 1)>...>;
        };

        // this expands to:
        // tuple<composition_map_t<1>, ..., composition_map_t<D>
        using composition_tuple_t = typename composition_tuple<>::type;

      public:
        SimplexFactory() = delete;

        template <int I>
        static simplex_t<I> * Simplex(const simplex_composition_t<I> & composition)
        {
            // pick a representative (factor out equivalence relation)
            auto representative = composition;
            _representative(representative);

            // if there is no representative registered in the map, then create a new simplex 
            // with this composition
            auto ret = std::get<I - 1>(_compositions)
                           .emplace(representative, new simplex_t<I>(composition));

            // return representative of simplex with composition {composition} 
            return ret.first->second;
        }

      private:
        // equivalence class relation for a simplex in 1D
        static void _representative(simplex_composition_t<1> & composition)
        {
            // pick a representative (factor out equivalence relation)
            std::sort(composition.begin(), composition.end());
            // all done
            return;
        }

        // equivalence class relation for a simplex in 2D
        static void _representative(simplex_composition_t<2> & composition)
        {
            // pick a representative (factor out equivalence relation)
            auto first_simplex = std::min_element(composition.begin(), composition.end());
            std::rotate(composition.begin(), first_simplex, composition.end());
            // all done
            return;
        }

      private:
        // container to store D maps with the composition of i-dimensional simplices in terms
        // of arrays of (i-1)-dimensional simplices
        static composition_tuple_t _compositions;
    };

    // initialize static attribute
    template <int D>
    mito::mesh::SimplexFactory<D>::composition_tuple_t
        mito::mesh::SimplexFactory<D>::_compositions =
            mito::mesh::SimplexFactory<D>::composition_tuple_t();
}

#endif    // mito_mesh_SimplexFactory_h

// end of file
