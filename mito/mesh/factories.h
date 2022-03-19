// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args)
    {
        return point_t<D>(std::forward<Args>(args)...);
    }

    // vertex factory
    template <class... Args>
    constexpr auto vertex(Args &&... args)
    {
        return vertex_t(std::forward<Args>(args)...);
    }

    // segment factory
    constexpr auto segment(const std::array<vertex_t *, 2> & simplices)
    {
        return segment_t(simplices);
    }
    constexpr auto segment(std::array<vertex_t *, 2> && simplices)
    {
        return segment_t(std::move(simplices));
    }

    // triangle factory
    constexpr auto triangle(const std::array<segment_t *, 3> & simplices)
    {
        return triangle_t(simplices);
    }
    constexpr auto triangle(std::array<segment_t *, 3> && simplices)
    {
        return triangle_t(std::move(simplices));
    }

    // tetrahedron factory
    constexpr auto tetrahedron(const std::array<triangle_t *, 4> & simplices)
    {
        return tetrahedron_t(simplices);
    }
    constexpr auto tetrahedron(std::array<triangle_t *, 4> && simplices)
    {
        return tetrahedron_t(std::move(simplices));
    }

    // vertex set factory
    template <int D>
    constexpr auto vertex_set()
    {
        return vertex_set_t<D>();
    }

    // element set factory
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
    }

    // vertex set factory
    template <int D>
    auto mesh(std::string meshFileName)
    {
        return mesh_t<D>(meshFileName);
    }
}


#endif

// end of file
