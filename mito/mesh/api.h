// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point alias
    template <int D>
    using point_t = vector_t<D>;    // Point<D>;

    // simplex alias
    template <int D>
    using simplex_t = Simplex<D>;

    // vertex alias
    using vertex_t = Simplex<0>;

    // segment alias
    using segment_t = Simplex<1>;

    // triangle alias
    using triangle_t = Simplex<2>;

    // tetrahedron alias
    using tetrahedron_t = Simplex<3>;

    // vertex set alias
    template <int D>
    using vertex_set_t = VertexSet<D>;

    // element set alias
    template <class elementT, int D>
    using element_set_t = ElementSet<elementT, D>;

    // mesh alias
    template <int D>
    using mesh_t = Mesh<D>;
}


#endif

// end of file
