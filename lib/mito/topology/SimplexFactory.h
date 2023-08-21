// code guard
#if !defined(mito_topology_SimplexFactory_h)
#define mito_topology_SimplexFactory_h

namespace mito::topology {

    /**
     *
     * This class represents a factory for simplices of order N.
     *
     * The factory class is aware of the class of equivalence for a Simplex of order N. In fact,
     * being an instance of Simplex<N> identified by its N+1 subsimplices, there are (N+1)! possible
     * representations of the same simplex. The factory makes sure that at any given time
     * there is at most one simplex for an equivalence class, i.e. the representative of the class.
     * The representative of the class of equivalence is chosen by sorting in increasing order the
     * addresses of the instances of the subsimplices.
     */

    // TOFIX: rename this to UnorientedSimplexFactory

    template <int N>
    class SimplexFactory {

      private:
        // typedef for a repository of unoriented simplices
        using simplex_repository_t = utilities::repository_t<unoriented_simplex_t<N>>;

        // id type of oriented simplex
        using unoriented_simplex_id_t = utilities::index_t<simplex_t<N>>;

        // typedef for a composition map of simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // edges composition
        // std::map<std::array<unoriented_simplex_id_t<N>, 2>, unoriented_simplex_t<1>>
        // faces compositions
        // std::map<std::array<unoriented_simplex_id_t<N>, 3>, unoriented_simplex_t<2>>
        // volumes compositions
        // std::map<std::array<unoriented_simplex_id_t<N>, 4>, unoriented_simplex_t<3>>
        using composition_t = std::array<unoriented_simplex_id_t, N + 1>;
        using composition_map_t = std::map<composition_t, unoriented_simplex_id_t>;

      private:
        // default constructor
        SimplexFactory() : _simplices(100 /*segment size */), _compositions() {};

        // destructor
        ~SimplexFactory() {}

        inline auto findSimplex(const simplex_composition_t<N> & composition) const
            -> unoriented_simplex_t<N>
        {
            // pick a representative (factor out equivalence relation)
            auto representative = _representative(composition);

            // look up for this representative in the compositions map
            auto it_find = _compositions.find(representative);

            // if a representative simplex with this composition is already registered in the map
            if (it_find != std::end(_compositions)) {
                // return it
                return _simplices.resource(it_find->second);
            }
            // if not found
            else {
                // return a shared pointer wrapper around {nullptr}
                return unoriented_simplex_t<N>();
            }
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        inline auto simplex(const simplex_composition_t<N> & composition) -> unoriented_simplex_t<N>
        {
            // pick a representative (factor out equivalence relation)
            auto representative = _representative(composition);

            // look up for this representative in the compositions map
            auto it_find = _compositions.find(representative);

            // if a representative simplex with this composition is already registered in the map
            if (it_find != std::end(_compositions)) {
                // then return it
                return _simplices.resource(it_find->second);
            }
            // otherwise
            else {

                // emplace simplex in {_simplices}
                auto simplex = _simplices.emplace(composition);

                // register the simplex in the compositions map
                _compositions.insert(std::make_pair(representative, simplex.id()));

                // and return it
                return simplex;
            }
        }

        // erase a simplex from the factory (this method actually erases the simplex only if there
        // is no one else using it, otherwise does nothing)
        inline auto erase(unoriented_simplex_t<N> & simplex) -> void
        {
            // sanity check
            assert(simplex.references() > 0);

            // pick a representative (factor out equivalence relation)
            auto representative = _representative(simplex->composition());

            // erase this simplex from the compositions map
            _compositions.erase(representative);

            // erase this simplex from the repository
            _simplices.erase(simplex);

            // all done
            return;
        }

      private:
        // equivalence class relation for a simplex
        inline auto _representative(const simplex_composition_t<N> & composition) const
            -> composition_t;

      private:
        // repository to store the unoriented simplices
        simplex_repository_t _simplices;

        // container to map simplex composition to simplices
        composition_map_t _compositions;

        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<N>;
    };

    // equivalence class relation for a simplex in 1D
    template <>
    auto SimplexFactory<1>::_representative(const simplex_composition_t<1> & composition) const
        -> composition_t
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint().id(),
                                       composition[1]->footprint().id() };
        // pick a representative (factor out equivalence relation)
        std::sort(std::begin(representative), std::end(representative));
        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 2D
    template <>
    auto SimplexFactory<2>::_representative(const simplex_composition_t<2> & composition) const
        -> composition_t
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint().id(),
                                       composition[1]->footprint().id(),
                                       composition[2]->footprint().id() };

        // pick a representative (factor out equivalence relation)
        std::sort(std::begin(representative), std::end(representative));

        // all done
        return representative;
    }

    // equivalence class relation for a simplex in 3D
    template <>
    auto SimplexFactory<3>::_representative(const simplex_composition_t<3> & composition) const
        -> composition_t
    {
        // initialize representative with footprints of simplices in current composition
        composition_t representative { composition[0]->footprint().id(),
                                       composition[1]->footprint().id(),
                                       composition[2]->footprint().id(),
                                       composition[3]->footprint().id() };

        // pick a representative (factor out equivalence relation)
        std::sort(std::begin(representative), std::end(representative));

        // all done
        return representative;
    }

    template <>
    class SimplexFactory<0> {

      private:
        // typedef for a collection of unoriented simplices
        using simplex_repository_t = utilities::repository_t<unoriented_simplex_t<0>>;

      private:
        // default constructor
        SimplexFactory() : _simplices(100 /*segment size */) {};

        // destructor
        ~SimplexFactory() {}

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        inline auto simplex() -> unoriented_simplex_t<0>
        {
            // emplace the new vertex in the vertex repository and return it
            return _simplices.emplace();
        }

        // erase a simplex from the factory (this method actually erases the simplex only if there
        // is no one else using it, otherwise does nothing)
        inline auto erase(unoriented_simplex_t<0> & simplex) -> void
        {
            // sanity check
            assert(simplex.references() > 0);

            // erase this simplex from the container
            _simplices.erase(simplex);

            // all done
            return;
        }

      private:
        // repository to store the unoriented simplices
        simplex_repository_t _simplices;

        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<0>;
    };
}

#endif    // mito_topology_SimplexFactory_h

// end of file
