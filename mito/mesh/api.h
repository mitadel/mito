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
    constexpr auto segment(const std::array<vertex_t *, 2> & simplices)
    {
        return segment_t(simplices);
    }
    constexpr auto segment(const std::array<vertex_t *, 2> && simplices)
    {
        return segment_t(std::move(simplices));
    }

    // triangle alias
    using triangle_t = Simplex<2>;

    // triangle factory
    constexpr auto triangle(const std::array<segment_t *, 3> & simplices)
    {
        return triangle_t(simplices);
    }
    constexpr auto triangle(const std::array<segment_t *, 3> && simplices)
    {
        return triangle_t(std::move(simplices));
    }

    // tetrahedron alias
    using tetrahedron_t = Simplex<3>;

    // tetrahedron factory
    constexpr auto tetrahedron(const std::array<triangle_t *, 4> & simplices)
    {
        return tetrahedron_t(simplices);
    }
    constexpr auto tetrahedron(const std::array<triangle_t *, 4> && simplices)
    {
        return tetrahedron_t(std::move(simplices));
    }

    // vertex set alias
    template <int D>
    using vertex_set_t = VertexSet<D>;

    // vertex set factory
    template <int D>
    constexpr auto vertex_set()
    {
        return vertex_set_t<D>();
    }

    // element set alias
    template <class elementT, int D>
    using element_set_t = ElementSet<elementT, D>;

    // element set factory
    template <class elementT, int D, template <class> class CONTAINER>
    constexpr auto element_set(CONTAINER<elementT *> && elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::forward<CONTAINER<elementT *>>(elements), vertices);
    }

    // mesh alias
    template <int D>
    using mesh_t = Mesh<D>;

    // vertex set factory
    template <int D>
    constexpr auto mesh(std::string meshFileName)
    {
        return mesh_t<D>(meshFileName);
    }
}


#endif

// end of file
