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

    // class point cloud
    template <int D>
    class PointCloud;

    // point cloud alias
    template <int D>
    using point_cloud_t = PointCloud<D>;

    // element set alias
    template <class elementT>
    using simplex_set_t = std::unordered_set<elementT *>;

    // element vector alias
    template <class elementT>
    using simplex_vector_t = std::vector<elementT *>;

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

#if 1
    // helper class to allow template specialization of oriented simplex composition alias
    template <int D>
    class helperOrientedSimplexCompositionClass {
      public:
        using oriented_simplex_composition_type =
            std::array<std::shared_ptr<oriented_simplex_t<D - 1>>, D + 1>;
    };

    // helper class to allow template specialization of oriented simplex composition alias
    template <>
    class helperOrientedSimplexCompositionClass<1> {
      public:
        using oriented_simplex_composition_type = std::array<simplex_t<0> *, 2>;
    };

    // oriented simplex composition alias
    template <int D>
    using oriented_simplex_composition_t =
        typename helperOrientedSimplexCompositionClass<D>::oriented_simplex_composition_type;
#else

    template <int D>
    using oriented_simplex_composition_t =
        std::array<std::shared_ptr<oriented_simplex_t<D - 1>>, D + 1>;
#endif

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
