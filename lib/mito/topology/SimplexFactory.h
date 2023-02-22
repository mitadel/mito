// code guard
#if !defined(mito_topology_SimplexFactory_h)
#define mito_topology_SimplexFactory_h

namespace mito::topology {

    /**
     *
     * This class represents a factory for simplices of order D.
     *
     * The factory class is aware of the class of equivalence for a Simplex of order D. In fact,
     * being an instance of Simplex<D> identified by its D+1 subsimplices, there are (D+1)! possible
     * representations of the same simplex. The factory makes sure that at any given time
     * there is at most one simplex for an equivalence class, i.e. the representative of the class.
     * The representative of the class of equivalence is chosen by sorting in increasing order the
     * addresses of the instances of the subsimplices.
     */

    // TOFIX: rename this to UnorientedSimplexFactory

    template <int D>
    class SimplexFactory {

      private:
        // typedef for a collection of unoriented simplices
        using simplex_collection_t = mito::utilities::segmented_t<unoriented_simplex_t<D>>;

        // typedef for a composition map of simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // edges composition
        // std::map<std::array<unoriented_simplex_id_t, 2>, unoriented_simplex_t<1>>
        // faces compositions
        // std::map<std::array<unoriented_simplex_id_t, 3>, unoriented_simplex_t<2>>
        // volumes compositions
        // std::map<std::array<unoriented_simplex_id_t, 4>, unoriented_simplex_t<3>>
        using composition_t = std::array<unoriented_simplex_id_t, D + 1>;
        using composition_map_t = std::map<composition_t, unoriented_simplex_t<D>>;

      private:
        // default constructor
        SimplexFactory() : _simplices(100 /*segment size */), _compositions() {};

        // destructor
        ~SimplexFactory() {};

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        inline auto simplex(const simplex_composition_t<D> & composition)
            -> const unoriented_simplex_t<D> &
        {
            // pick a representative (factor out equivalence relation)
            auto representative = _representative(composition);

            // look up for this representative in the compositions map
            auto it_find = _compositions.find(representative);

            // if a representative simplex with this composition is already registered in the map
            if (it_find != _compositions.end()) {
                // then return it
                return it_find->second;
            }
            // otherwise
            else {
                // create a new simplex with composition {composition}
                auto simplex = _simplices.emplace(composition);

                // register it in the compositions map
                auto ret = _compositions.insert(std::make_pair(representative, simplex));

                // and return it
                return ret.first->second;
            }
        }

        // erase a simplex from the factory (this method actually erases the simplex only if there
        // is no one else using it, otherwise does nothing)
        inline auto erase(const unoriented_simplex_t<D> & simplex) -> void
        {
            // sanity check
            assert(simplex.references() > 0);

            // pick a representative (factor out equivalence relation)
            auto representative = _representative(simplex->composition());

            // erase it
            simplex->_erase();

            // erase this simplex from the compositions map
            _compositions.erase(representative);

            // all done
            return;
        }

      private:
        // equivalence class relation for a simplex
        inline auto _representative(const simplex_composition_t<D> & composition) -> auto;

      private:
        // container to store the unoriented simplices
        simplex_collection_t _simplices;

        // container to map simplex composition to simplices
        composition_map_t _compositions;

        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<D>;
    };

    // equivalence class relation for a simplex in 1D
    template <>
    auto SimplexFactory<1>::_representative(const simplex_composition_t<1> & composition) -> auto
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint_id(),
                                       composition[1]->footprint_id() };
        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());
        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 2D
    template <>
    auto SimplexFactory<2>::_representative(const simplex_composition_t<2> & composition) -> auto
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint_id(),
                                       composition[1]->footprint_id(),
                                       composition[2]->footprint_id() };

        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());

        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 3D
    template <>
    auto SimplexFactory<3>::_representative(const simplex_composition_t<3> & composition) -> auto
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint_id(),
                                       composition[1]->footprint_id(),
                                       composition[2]->footprint_id(),
                                       composition[3]->footprint_id() };

        // pick a representative (factor out equivalence relation)
        std::sort(representative.begin(), representative.end());

        // all done
        return representative;
    }

    template <>
    class SimplexFactory<0> {

      private:
        // typedef for a collection of unoriented simplices
        using simplex_collection_t = mito::utilities::segmented_t<unoriented_simplex_t<0>>;

      private:
        // default constructor
        SimplexFactory() : _simplices(100 /*segment size */) {};

        // destructor
        ~SimplexFactory() {};

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        inline auto simplex() -> const unoriented_simplex_t<0> &
        {
            // emplace the new vertex in the vertex collection and return it
            return *_vertex_set.insert(_simplices.emplace()).first;
        }

        // erase a simplex from the factory (this method actually erases the simplex only if there
        // is no one else using it, otherwise does nothing)
        inline auto erase(const unoriented_simplex_t<0> & simplex) -> void
        {

            // sanity check
            assert(simplex.references() > 0);

            // erase the simplex
            simplex->_erase();

            // erase the vertex from the vertex set
            _vertex_set.erase(simplex);

            // all done
            return;
        }

      private:
        // container to store the unoriented simplices
        simplex_collection_t _simplices;

        // container for persistent storage of the shared pointers to vertices
        element_set_t<unoriented_simplex_t<0>> _vertex_set;

        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<0>;
    };
}

#endif    // mito_topology_SimplexFactory_h

// end of file
