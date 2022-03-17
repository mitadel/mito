// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class element set
    template <class element_t, int D>
    class ElementSet;

    // element set alias
    template <class elementT, int D>
    using element_set_t = ElementSet<elementT, D>;

    // class mesh
    template <int D>
    class Mesh;

    // mesh alias
    template <int D>
    using mesh_t = Mesh<D>;

    // class point
    template <int D>
    class Point;

    // point alias
    template <int D>
    using point_t = vector_t<D>;    // Point<D>;

    // class simplex
    template <int D>
    class Simplex;

    // class oriented simplex
    template <int D>
    class OrientedSimplex;

    // simplex alias
    template <int D>
    using simplex_t = Simplex<D>;

    // vertex alias
    using vertex_t = Simplex<0>;

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = OrientedSimplex<D>;

    // QUESTION: in the API should segment be an alias for Simplex<1> or OrientedSimplex<1>?
    // segment alias
    using segment_t = OrientedSimplex<1>;

    // triangle alias
    using triangle_t = OrientedSimplex<2>;

    // tetrahedron alias
    using tetrahedron_t = OrientedSimplex<3>;

    // class vertex set
    template <int D>
    class VertexSet;

    // vertex set alias
    template <int D>
    using vertex_set_t = VertexSet<D>;

}


#endif

// end of file
