// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point alias
    template <int D>
    using point_t = vector_t<D>;    // Point<D>;

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args)
    {
        return point_t<D>(std::forward<Args>(args)...);
    }

    // vertex alias
    using vertex_t = Simplex<0>;

    // vertex factory
    template <class... Args>
    constexpr auto vertex(Args &&... args)
    {
        return vertex_t(std::forward<Args>(args)...);
    }

    // segment alias
    using segment_t = Simplex<1>;

    // segment factory
    template <class... Args>
    constexpr auto segment(Args &&... args)
    {
        return segment_t(std::forward<Args>(args)...);
    }

    // triangle alias
    using triangle_t = Simplex<2>;

    // triangle factory
    template <class... Args>
    constexpr auto triangle(Args &&... args)
    {
        return triangle_t(std::forward<Args>(args)...);
    }

    // tetrahedron alias
    using tetrahedron_t = Simplex<3>;

    // tetrahedron factory
    template <class... Args>
    constexpr auto tetrahedron(Args &&... args)
    {
        return tetrahedron_t(std::forward<Args>(args)...);
    }

    // element set alias
    template <class elementT, int D>
    using element_set_t = ElementSet<elementT, D>;

    // element set factory
    template <class elementT, int D, class... Args>
    constexpr auto element_set(Args &&... args)
    {
        return element_set_t<elementT, D>(std::forward<Args>(args)...);
    }

    // vertex set alias
    template <int D>
    using vertex_set_t = VertexSet<D>;

    // vertex set factory
    template <int D, class... Args>
    constexpr auto vertex_set(Args &&... args)
    {
        return vertex_set_t<D>(std::forward<Args>(args)...);
    }

    // mesh alias
    template <int D>
    using mesh_t = Mesh<D>;

    // vertex set factory
    template <int D, class... Args>
    constexpr auto mesh(Args &&... args)
    {
        return mesh_t<D>(std::forward<Args>(args)...);
    }
}


#endif

// end of file
