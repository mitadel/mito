// code guard
#if !defined(mito_topology_Topology_h)
#define mito_topology_Topology_h

namespace mito::topology {

    /**
     *
     *
     */

    class Topology {
      public:
        // default constructor
        Topology() : _factories() {};

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        template <int D>
        inline auto simplex(const simplex_composition_t<D> & composition) -> simplex_t<D>
        requires(D > 0)
        {
            // ask the factory of oriented simplices
            return std::get<D>(_factories).orientedSimplex(composition);
        }

        template <int D>
        inline auto simplex() -> simplex_t<0>
        requires(D == 0)
        {
            // ask the factory of oriented simplices
            return std::get<0>(_factories).orientedSimplex();
        }

        // instantiate a vertex
        inline auto vertex() -> simplex_t<0> { return simplex<0>(); }

        // instantiate a segment
        inline auto segment(const simplex_composition_t<1> & simplices) -> simplex_t<1>
        {
            return simplex<1>(simplices);
        }

        // instantiate a triangle
        inline auto triangle(const simplex_composition_t<2> & simplices) -> simplex_t<2>
        {
            return simplex<2>(simplices);
        }

        // instantiate a tetrahedron
        inline auto tetrahedron(const simplex_composition_t<3> & simplices) -> simplex_t<3>
        {
            return simplex<3>(simplices);
        }

        // TOFIX: change name, this is not actually the incidence
        // returns the number of owners of the shared pointer to this oriented simplex
        template <int D>
        inline auto incidence(const simplex_t<D> & simplex) const -> int
        {
            // ask the factory of oriented simplices
            return std::get<D>(_factories).incidence(simplex);
        }

        // returns whether there exists the flipped oriented simplex in the factory
        template <int D>
        inline auto exists_flipped(const simplex_t<D> & simplex) const -> bool
        {
            // get the incidence of the simplex footprint
            auto inc = incidence(simplex);
            // assert the footprint cannot be used by more than two oriented simplices
            assert(inc == 1 || inc == 2);
            // return true if the footprint is in used by two oriented simplices
            return inc == 2 ? true : false;
        }

        // returns the simplex with opposite orientation
        template <int D>
        inline auto flip(const simplex_t<D> & simplex) -> simplex_t<D>
        {
            // ask the factory of oriented simplices
            return std::get<D>(_factories)
                .orientedSimplex(simplex->footprint(), !simplex->orientation());
        }

        template <int D>
        inline auto cleanup(const simplex_t<D> & simplex) -> void
        {
            // cleanup the oriented factory around {simplex}
            return std::get<D>(_factories).cleanup(simplex);
        }

      private:
        std::tuple<
            oriented_simplex_factory_t<0>, oriented_simplex_factory_t<1>,
            oriented_simplex_factory_t<2>, oriented_simplex_factory_t<3>>
            _factories;
    };
}


#endif    // mito_topology_Topology_h

// end of file
