// code guard
#if !defined(mito_topology_Topology_h)
#define mito_topology_Topology_h

namespace mito::topology {

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
        static inline auto orientedSimplex(const simplex_composition_t<D> & composition)
            -> simplex_t<D>
        requires(D > 0)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::orientedSimplex(composition);
        }

        static inline auto orientedSimplex() -> simplex_t<0>
        requires(D == 0)
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<0>::orientedSimplex();
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        static inline auto incidence(const simplex_t<D> & oriented_simplex) -> int
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::incidence(oriented_simplex);
        }

        // returns whether there exists the flipped oriented simplex in the factory
        static inline auto exists_flipped(const simplex_t<D> & oriented_simplex) -> bool
        {
            // get the incidence of the simplex footprint
            auto inc = incidence(oriented_simplex);
            // assert the footprint cannot be used by more than two oriented simplices
            assert(inc == 1 || inc == 2);
            // return true if the footprint is in used by two oriented simplices
            return inc == 2 ? true : false;
        }

        // returns the simplex with opposite orientation
        static inline auto flip(const simplex_t<D> & oriented_simplex) -> simplex_t<D>
        {
            // ask the factory of oriented simplices
            return OrientedSimplexFactory<D>::orientedSimplex(
                oriented_simplex->footprint(), !oriented_simplex->orientation());
        }

        static inline auto cleanup(const simplex_t<D> & oriented_simplex) -> void
        {
            // cleanup the oriented factory around {oriented_simplex}
            return OrientedSimplexFactory<D>::cleanup(oriented_simplex);
        }
    };

}


#endif    // mito_topology_Topology_h

// end of file
