// code guard
#if !defined(mito_topology_OrientedSimplexFactory_h)
#define mito_topology_OrientedSimplexFactory_h

namespace mito::topology {

    /**
     *
     * This static class represents a factory for oriented simplices of order D.
     *
     * The factory class is aware of the class of equivalence for an OrientedSimplex of order D. In
     * fact, being an instance of OrientedSimplex<D> identified by its D+1 subsimplices (through its
     * footprint), there are (D+1)!/2 possible representations of the same oriented simplex. The
     * factory makes sure that at any given time there is at most one OrientedSimplex for an
     * equivalence class, i.e. the representative of the class. The representative of the class of
     * equivalence is chosen by starting the composition with the subsimplex of smallest address and
     * by circularly rotating the other simplices in the composition array around the first simplex.
     */

    template <int D>
    class OrientedSimplexFactory {
      private:
        // typedef for an orientation map of simplices:
        // this map maps a simplex pointer and a boolean to an oriented simplex pointer
        using orientation_map_t =
            std::map<std::tuple<const simplex_t<D> *, bool>, oriented_simplex_ptr<D>>;

      public:
        // delete default constructor
        OrientedSimplexFactory() = delete;

        // return an oriented simplex riding on footprint {simplex} and with orientation
        // {orientation} (either create a new oriented simplex if such oriented simplex does not
        // exist in the factory or return the existing representative of the class of equivalence of
        // oriented simplices with footprint {simplex} orientation {orientation}
        static oriented_simplex_ptr<D> orientedSimplex(
            const simplex_ptr<D> & simplex, bool orientation)
        {

            // bind the footprint and the orientation in a tuple
            auto mytuple = std::make_tuple(simplex.get(), orientation);

            // if there is no oriented simplex registered in the map riding on simplex {simplex},
            // with orientation {orientation} then create such new oriented simplex and register it
            // in the map
            auto ret =
                _orientations.emplace(mytuple, new oriented_simplex_t<D>(simplex, orientation));

            // return representative of simplex with composition {composition}
            return ret.first->second;
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        static oriented_simplex_ptr<D> orientedSimplex(const simplex_composition_t<D> & composition)
        {
            // get from the factory the representative of simplices with this composition
            auto simplex = SimplexFactory<D>::simplex(composition);

            // compute the orientation of the current composition with respect to the representative
            bool orientation = _orientation(composition, simplex);

            // return an oriented simplex riding on {simplex} with {orientation}
            return orientedSimplex(simplex, orientation);
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        static int incidence(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            return oriented_simplex.use_count() - 1;
        }

        static void _cleanup(const oriented_simplex_ptr<D> & oriented_simplex, int i = 0)
        {
            // if the oriented simplex is unused
            if (incidence(oriented_simplex) == i) {

                // fetch subsimplices before doing any harm to the oriented simplex
                auto subsimplices = oriented_simplex->simplices();

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
                    // TOFIX: because the subsimplices are fetched by copy (they cannot be fetched
                    // by reference because the {oriented_simplex} is deleted after being erased
                    // from the orientation map), we need to account for a {use_count} artificially
                    // increased by one, which is taken care of by passing 1 in the {_cleanup}
                    // function. This will be fixed once we pass to {mito::shared_ptr} instead of
                    // {std::shared_ptr}.
                    OrientedSimplexFactory<D - 1>::_cleanup(subsimplex, 1);
                }
            }

            // all done
            return;
        }

        // cleanup the factory around an oriented simplex (i.e. remove from the factory unused
        // oriented simplices related to this oriented simplex)
        static void cleanup(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // cleanup recursively until D = 0
            _cleanup(oriented_simplex);

            // all done
            return;
        }

      private:
        // compute the orientation of the {composition} with respect to the orientation of
        // {simplex}
        static bool _orientation(
            const simplex_composition_t<D> & composition, const simplex_ptr<D> & simplex);

      private:
        // container to store the relation (simplex, orientation) -> oriented simplex
        static orientation_map_t _orientations;
    };

    // compute the orientation of the {composition} with respect to the orientation of {simplex}
    template <>
    bool OrientedSimplexFactory<1>::_orientation(
        const simplex_composition_t<1> & composition, const simplex_ptr<1> & simplex)
    {
        if (composition == simplex->simplices()) {
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
        const simplex_composition_t<2> & composition, const simplex_ptr<2> & simplex)
    {
        if (_rotate(composition) == _rotate(simplex->simplices())) {
            return true;
        }
        return false;
    }

    template <>    // TODO: implement
    bool OrientedSimplexFactory<3>::_orientation(
        const simplex_composition_t<3> & composition, const simplex_ptr<3> & simplex)
    {
        return true;
    }

    // initialize static attribute
    template <int D>
    typename OrientedSimplexFactory<D>::orientation_map_t OrientedSimplexFactory<D>::_orientations =
        OrientedSimplexFactory<D>::orientation_map_t();

    /*
     * This class specializes OrientedSimplexFactory<D> for D = 0.
     */
    template <>
    class OrientedSimplexFactory<0> {
      private:
        using vertex_collection_t = simplex_set_t<oriented_simplex_t<0>>;

      public:
        // delete default constructor
        OrientedSimplexFactory() = delete;

        // adds a new vertex to the vertex collection and returns it
        static oriented_simplex_ptr<0> orientedSimplex()
        {
            // insert the new vertex in the vertex set
            auto ret = _vertices.insert(std::make_shared<oriented_simplex_t<0>>());

            // return vertex
            return *ret.first;
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        static int incidence(const oriented_simplex_ptr<0> & oriented_simplex)
        {
            return oriented_simplex.use_count() - 1;
        }

        static void _cleanup(const oriented_simplex_ptr<0> & oriented_simplex, int i = 0)
        {
            // if the oriented simplex is unused
            if (incidence(oriented_simplex) == i) {
                // erase this oriented simplex from the oriented simplex factory
                _vertices.erase(oriented_simplex);
                // TOFIX: at this point it should also be checked whether the vertex should be
                // erased from the PointCloud too. However, at this point in the code we do not
                // have the information of the spatial dimension D. In alternative this cleanup
                // can be done in the mesh but with the current implementation this is not possible
                // as we would need to loop on the subsimplices of a deleted simplex
            }

            // all done
            return;
        }

      private:
        // container to store the vertices
        static vertex_collection_t _vertices;
    };

    // initialize static attribute
    typename OrientedSimplexFactory<0>::vertex_collection_t OrientedSimplexFactory<0>::_vertices =
        OrientedSimplexFactory<0>::vertex_collection_t();
}

#endif    // mito_topology_OrientedSimplexFactory_h

// end of file
