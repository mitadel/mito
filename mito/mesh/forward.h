// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

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

    // class simplex factory
    template <int D>
    class SimplexFactory;

    // class oriented simplex factory
    template <int D>
    class OrientedSimplexFactory;

    // simplex alias
    template <int D>
    using simplex_t = Simplex<D>;

    // alias for simplex composition
    template <int D>
    using simplex_composition_t = std::array<simplex_t<D - 1> *, D + 1>;

    template <int D>
    class OrientedSimplexComposition;

    // alias for oriented simplex composition
    template <int D>
    using oriented_simplex_composition_t = OrientedSimplexComposition<D>;

    // vertex alias
    using vertex_t = Simplex<0>;

    // helper class to allow template specialization of oriented simplex alias
    template <int D>
    class helperOrientedSimplexClass {
      public:
        using simplex_type = OrientedSimplex<D>;
    };

    // helper class to allow template specialization of oriented simplex alias
    template <>
    class helperOrientedSimplexClass<0> {
      public:
        using simplex_type = Simplex<0>;
    };

    // oriented simplex alias
    template <int D>
    using oriented_simplex_t = typename helperOrientedSimplexClass<D>::simplex_type;

    // QUESTION: in the API should segment be an alias for Simplex<1> or OrientedSimplex<1>?
    // segment alias
    using segment_t = OrientedSimplex<1>;

    // triangle alias
    using triangle_t = OrientedSimplex<2>;

    // tetrahedron alias
    using tetrahedron_t = OrientedSimplex<3>;

    // class vertex set
    template <int D>
    class PointCloud;

    // vertex set alias
    template <int D>
    using point_cloud_t = PointCloud<D>;

}


#endif

// end of file
