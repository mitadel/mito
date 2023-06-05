// code guard
#if !defined(mito_topology_Topology_h)
#define mito_topology_Topology_h

namespace mito::topology {

    /**
     *
     *
     */

    class Topology {
      private:
        // default constructor
        Topology() :
            _vertex_factory(),
            _segment_factory(),
            _triangle_factory(),
            _tetrahedron_factory() {};

        // delete copy constructor
        Topology(const Topology &) = delete;

        // delete assignment operator
        void operator=(const Topology &) = delete;

        // destructor
        ~Topology() {};

      public:
        template <int D>
        inline auto simplex(const unoriented_simplex_t<D> & footprint, bool orientation)
            -> const simplex_t<D> &
        {
            // ask the factory of oriented simplices
            return _get_factory<D>().orientedSimplex(footprint, orientation);
        }

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        template <int D>
        inline auto simplex(const simplex_composition_t<D> & composition) -> const simplex_t<D> &
        requires(D > 0)
        {
            // ask the factory of oriented simplices
            return _get_factory<D>().orientedSimplex(composition);
        }

        template <int D>
        inline auto simplex(bool orientation) -> const simplex_t<0> &
        requires(D == 0)
        {
            // ask the factory of oriented simplices
            return _get_factory<0>().orientedSimplex(orientation);
        }

        // instantiate a vertex
        inline auto vertex() -> const vertex_t &;

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
            return _get_factory<D>().existsOrientedSimplex(
                simplex->footprint(), simplex->orientation());
        }

        // returns whether there exists the flipped oriented simplex in the factory
        template <int D>
        inline auto exists_flipped(const simplex_t<D> & simplex) const -> bool
        {
            return _get_factory<D>().existsOrientedSimplex(
                simplex->footprint(), !simplex->orientation());
        }

        // returns the simplex with opposite orientation
        template <int D>
        inline auto flip(const simplex_t<D> & simplex) -> const simplex_t<D> &
        {
            // ask the factory of oriented simplices
            return _get_factory<D>().orientedSimplex(simplex->footprint(), !simplex->orientation());
        }

      private:
        template <int D>
        inline auto _erase(const simplex_t<D> & simplex) -> void
        requires(D == 0)
        {
            // erase the vertex from the factory
            _get_factory<D>().erase(simplex);

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
            _get_factory<D>().erase(simplex);

            // loop on subsimplices
            for (const auto & subsimplex : composition) {
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

        // mutator for the simplex factory of dimension D
        template <int D>
        inline auto _get_factory() -> oriented_simplex_factory_t<D> &;

        // accessor for the simplex factory of dimension D
        template <int D>
        inline auto _get_factory() const -> const oriented_simplex_factory_t<D> &;

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
        // factory for vertices
        oriented_simplex_factory_t<0> _vertex_factory;

        // factory for segments
        oriented_simplex_factory_t<1> _segment_factory;

        // factory for triangles
        oriented_simplex_factory_t<2> _triangle_factory;

        // factory for tetrahedra
        oriented_simplex_factory_t<3> _tetrahedron_factory;

        // friendship with the singleton
        using TopologySingleton = utilities::Singleton<Topology>;
        friend TopologySingleton;
    };
}

// TOFIX: split header into {Topology.h} and {Topology.icc} so these can be moved to the icc file
template <>
inline auto
mito::topology::Topology::_get_factory<0>() -> oriented_simplex_factory_t<0> &
{
    return _vertex_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<1>() -> oriented_simplex_factory_t<1> &
{
    return _segment_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<2>() -> oriented_simplex_factory_t<2> &
{
    return _triangle_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<3>() -> oriented_simplex_factory_t<3> &
{
    return _tetrahedron_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<0>() const -> const oriented_simplex_factory_t<0> &
{
    return _vertex_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<1>() const -> const oriented_simplex_factory_t<1> &
{
    return _segment_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<2>() const -> const oriented_simplex_factory_t<2> &
{
    return _triangle_factory;
}

template <>
inline auto
mito::topology::Topology::_get_factory<3>() const -> const oriented_simplex_factory_t<3> &
{
    return _tetrahedron_factory;
}

// instantiate a vertex
inline auto
mito::topology::Topology::vertex() -> const vertex_t &
{
    // ask the factory of vertices for an unoriented vertex
    return _get_factory<0>().vertex();
}

#endif    // mito_topology_Topology_h

// end of file
