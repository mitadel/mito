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
        inline auto simplex(const simplex_composition_t<D> & composition) -> const simplex_t<D> &
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
        inline auto segment(const simplex_composition_t<1> & simplices) -> const simplex_t<1> &
        {
            return simplex<1>(simplices);
        }

        // instantiate a triangle
        inline auto triangle(const simplex_composition_t<2> & simplices) -> const simplex_t<2> &
        {
            return simplex<2>(simplices);
        }

        // instantiate a tetrahedron
        inline auto tetrahedron(const simplex_composition_t<3> & simplices) -> const simplex_t<3> &
        {
            return simplex<3>(simplices);
        }

        // returns whether there exists the flipped oriented simplex in the factory
        template <int D>
        inline auto exists_flipped(const simplex_t<D> & simplex) const -> bool
        {
            return std::get<D>(_factories)
                .existsOrientedSimplex(simplex->footprint(), !simplex->orientation());
        }

        // returns the simplex with opposite orientation
        template <int D>
        inline auto flip(const simplex_t<D> & simplex) -> const simplex_t<D> &
        {
            // ask the factory of oriented simplices
            return std::get<D>(_factories)
                .orientedSimplex(simplex->footprint(), !simplex->orientation());
        }

        template <int D>
        inline auto erase(const simplex_t<D> & simplex) -> void
        requires(D == 0)
        {
            // erase the vertex from the factory
            std::get<D>(_factories).erase(simplex);

            // all done
            return;
        }

        template <int D>
        inline auto erase(const simplex_t<D> & simplex) -> void
        requires(D > 0)
        {
            // sanity check
            assert(simplex.references() > 0);

            // // if someone else (other than the topology) is still using this resource
            // if (simplex.references() > 1) {
            //     // do nothing
            //     return;
            // }

            // grab a copy of the footprint
            auto footprint = simplex->footprint();

            // grab a copy of the composition
            auto composition = simplex->composition();

            // cleanup the oriented factory around {simplex}
            std::get<D>(_factories).erase(simplex);

            // if this simplex is the last one using the footprint
            if (footprint.references() == 2) {
                // cleanup the unoriented factory around {footprint}
                std::get<D>(_factories).erase(footprint);
            }

            // loop on subsimplices
            for (auto & subsimplex : composition) {
                // if this simplex is the last one using the subsimplex
                if (subsimplex.references() == 2) {
                    erase(subsimplex);
                }
            }

            // all done
            return;
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
