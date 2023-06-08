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
        Topology();

        // delete copy constructor
        Topology(const Topology &) = delete;

        // delete assignment operator
        void operator=(const Topology &) = delete;

        // destructor
        ~Topology();

      public:
        template <int D>
        inline auto simplex(const unoriented_simplex_t<D> & footprint, bool orientation)
            -> const simplex_t<D> &;

        // return a simplex with composition {composition} (either create a new simplex if such
        // simplex does not exist in the factory or return the existing representative of the class
        // of equivalence of simplices with this composition)
        template <int D>
        inline auto simplex(const simplex_composition_t<D> & composition) -> const simplex_t<D> &
        requires(D > 0);

        template <int D>
        inline auto simplex(bool orientation) -> const simplex_t<0> &
        requires(D == 0);

        // instantiate a vertex
        inline auto vertex() -> const vertex_t &;

        // instantiate a segment
        inline auto segment(const simplex_composition_t<1> & simplices) -> const simplex_t<1> &;

        // instantiate a triangle
        inline auto triangle(const simplex_composition_t<2> & simplices) -> const simplex_t<2> &;

        // instantiate a tetrahedron
        inline auto tetrahedron(const simplex_composition_t<3> & simplices) -> const simplex_t<3> &;

        // instantiate a segment from unoriented vertices
        inline auto segment(const vertex_simplex_composition_t<1> & simplices)
            -> const simplex_t<1> &;

        // instantiate a triangle
        inline auto triangle(const vertex_simplex_composition_t<2> & vertices)
            -> const simplex_t<2> &;

        // instantiate a tetrahedron
        inline auto tetrahedron(const vertex_simplex_composition_t<3> & vertices)
            -> const simplex_t<3> &;

        // returns whether the oriented simplex exists in the factory
        template <int D>
        inline auto exists(const simplex_composition_t<D> & simplices) const -> bool;

        // returns whether the segment exists in the factory
        inline auto exists(const vertex_simplex_composition_t<1> & vertices) const -> bool;

        // returns whether there exists the flipped oriented simplex in the factory
        template <int D>
        inline auto exists_flipped(const simplex_t<D> & simplex) const -> bool;

        // returns the simplex with opposite orientation
        template <int D>
        inline auto flip(const simplex_t<D> & simplex) -> const simplex_t<D> &;

      private:
        template <int D>
        inline auto _erase(const simplex_t<D> & simplex) -> void
        requires(D == 0);

        template <int D>
        inline auto _erase(const simplex_t<D> & simplex) -> void
        requires(D > 0);

        // mutator for the simplex factory of dimension D
        template <int D>
        inline auto _get_factory() -> oriented_simplex_factory_t<D> &;

        // accessor for the simplex factory of dimension D
        template <int D>
        inline auto _get_factory() const -> const oriented_simplex_factory_t<D> &;

      public:
        template <int D>
        inline auto erase(const simplex_t<D> & simplex) -> void;

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


// get the inline definitions
#define mito_topology_Topology_icc
#include "Topology.icc"
#undef mito_topology_Topology_icc

#endif    // mito_topology_Topology_h
// end of file
