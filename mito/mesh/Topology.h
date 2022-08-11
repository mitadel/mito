// code guard
#if !defined(mito_mesh_Topology_h)
#define mito_mesh_Topology_h

namespace mito::mesh {

    /**
     *
     *
     */

    template <int D>
    class Topology {
      public:
        // delete default constructor
        Topology() = delete;

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        static oriented_simplex_ptr<D> orientedSimplex(
            const simplex_composition_t<D> & composition) requires(D > 0)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::orientedSimplex(composition);
        }

        static oriented_simplex_ptr<0> orientedSimplex() requires (D == 0)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<0>::orientedSimplex();
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        static int incidence(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::incidence(oriented_simplex);
        }

        // returns whether there exists the flipped oriented simplex in the factory
        static bool exists_flipped(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // get the use count of the simplex footprint
            auto use_count = oriented_simplex->footprint().use_count();
            // assert the footprint cannot be used by more than two oriented simplices (on top of
            // the the SimplexFactory)
            assert(use_count == 2 || use_count == 3);
            // return true if the footprint is in used by two oriented simplices
            return use_count == 3 ? true : false;
        }

        // returns the simplex with opposite orientation
        static auto flip(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::orientedSimplex(
                oriented_simplex->footprint(), !oriented_simplex->orientation());
        }

        static void cleanup(const oriented_simplex_ptr<D> & oriented_simplex)
        {
            // cleanup the oriented factory around {oriented_simplex}
            return OrientedSimplexFactory<D>::cleanup(oriented_simplex);
        }
    };

}


#endif    // mito_mesh_Topology_h

// end of file
