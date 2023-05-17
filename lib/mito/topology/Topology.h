// code guard
#if !defined(mito_topology_Topology_h)
#define mito_topology_Topology_h

namespace mito::topology {

    /**
     *
     *
     */

    class Topology : public utilities::Singleton<Topology> {
      private:
        // default constructor
        Topology() : _factories() {};

        // delete copy constructor
        Topology(const Topology &) = delete;

        // delete assignment operator
        void operator=(const Topology &) = delete;

        // destructor
        ~Topology() override {};

      public:
        template <int D>
        inline auto simplex(const unoriented_simplex_t<D> & footprint, bool orientation)
            -> const simplex_t<D> &
        {
            // ask the factory of oriented simplices
            return std::get<D>(_factories).orientedSimplex(footprint, orientation);
        }

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
        inline auto simplex(bool orientation) -> const simplex_t<0> &
        requires(D == 0)
        {
            // ask the factory of oriented simplices
            return std::get<0>(_factories).orientedSimplex(orientation);
        }

        // instantiate a vertex
        inline auto vertex() -> const vertex_t &
        {
            // ask the factory of oriented simplices
            return simplex<0>(true)->footprint();
        }

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

        // instantiate a segment from unoriented vertices
        inline auto segment(const vertex_simplex_composition_t<1> & simplices)
            -> const simplex_t<1> &
        {
            return segment({ simplex(simplices[0], false), simplex(simplices[1], true) });
        }

        // instantiate a triangle
        inline auto triangle(const vertex_simplex_composition_t<2> & vertices)
            -> const simplex_t<2> &
        {
            return simplex<2>({ segment({ vertices[0], vertices[1] }),
                                segment({ vertices[1], vertices[2] }),
                                segment({ vertices[2], vertices[0] }) });
        }

        // instantiate a tetrahedron
        inline auto tetrahedron(const vertex_simplex_composition_t<3> & vertices)
            -> const simplex_t<3> &
        {
            return simplex<3>({ triangle({ vertices[0], vertices[1], vertices[3] }),
                                triangle({ vertices[1], vertices[2], vertices[3] }),
                                triangle({ vertices[2], vertices[0], vertices[3] }),
                                triangle({ vertices[0], vertices[2], vertices[1] }) });
        }

        // returns whether the oriented simplex exists in the factory
        template <int D>
        inline auto exists(const simplex_t<D> & simplex) const -> bool
        {
            return std::get<D>(_factories)
                .existsOrientedSimplex(simplex->footprint(), simplex->orientation());
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

      private:
        template <int D>
        inline auto _erase(const simplex_t<D> & simplex) -> void
        requires(D == 0)
        {
            // erase the vertex from the factory
            std::get<D>(_factories).erase(simplex);

            // all done
            return;
        }

        template <int D>
        inline auto _erase(const simplex_t<D> & simplex) -> void
        requires(D > 0)
        {
            // QUESTION: method {reset} is not {const} unless the {_handle} of the shared pointer is
            //          declared mutable. Should we call reset here and make the handle mutable or
            //          should we accept that {element} points to an invalid resource after call to
            //          {erase}?

            // sanity check
            assert(simplex.references() > 0);

            // grab a copy of the composition
            auto composition = simplex->composition();

            // cleanup the oriented factory around {simplex}
            std::get<D>(_factories).erase(simplex);

            // loop on subsimplices
            for (auto & subsimplex : composition) {
                // if this simplex is the last one using the subsimplex (other than the copy we just
                // did)
                if (subsimplex.references() == 2) {
                    // recursively erase the subsimplices
                    _erase(subsimplex);
                }
            }

            // all done
            return;
        }

      public:
        template <int D>
        inline auto erase(const simplex_t<D> & simplex) -> void
        {
            // if someone else (other than the topology) is still using this resource
            if (simplex.references() > 1) {
                // do nothing
                return;
            }

            // otherwise, erase the simplex
            return _erase(simplex);
        }

      private:
        std::tuple<
            oriented_simplex_factory_t<0>, oriented_simplex_factory_t<1>,
            oriented_simplex_factory_t<2>, oriented_simplex_factory_t<3>>
            _factories;

        // friendship with the singleton
        using TopologySingleton = utilities::Singleton<Topology>;
        friend TopologySingleton;
    };
}


#endif    // mito_topology_Topology_h

// end of file
