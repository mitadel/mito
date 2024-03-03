// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // topology factory
    inline auto topology() -> topology_t &
    {
        return utilities::Singleton<topology_t>::GetInstance();
    }

    // returns a new vertex
    inline auto vertex() -> vertex_t
    {
        return topology().vertex();
    }

    // returns the segment connecting {vertices}
    inline auto segment(const vertex_simplex_composition_t<1> & vertices) -> segment_t
    {
        return topology().segment(vertices);
    }

    // returns the triangle connecting {vertices}
    inline auto triangle(const vertex_simplex_composition_t<2> & vertices) -> triangle_t
    {
        return topology().triangle(vertices);
    }

    // returns the tetrahedron connecting {vertices}
    inline auto tetrahedron(const vertex_simplex_composition_t<3> & vertices) -> tetrahedron_t
    {
        return topology().tetrahedron(vertices);
    }

    // returns the segment connecting the oriented {vertices}
    inline auto segment(const simplex_composition_t<1> & vertices) -> segment_t
    {
        return topology().segment(vertices);
    }

    // returns the triangle connecting the oriented {segments}
    inline auto triangle(const simplex_composition_t<2> & segments) -> triangle_t
    {
        return topology().triangle(segments);
    }

    // returns the tetrahedron connecting the oriented {triangles}
    inline auto tetrahedron(const simplex_composition_t<3> & triangles) -> tetrahedron_t
    {
        return topology().tetrahedron(triangles);
    }
}

#endif

// end of file
