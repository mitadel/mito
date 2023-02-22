// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // vertex alias
    using vertex_t = unoriented_simplex_ptr<0>;

    // segment alias
    using segment_t = simplex_t<1>;

    // triangle alias
    using triangle_t = simplex_t<2>;

    // tetrahedron alias
    using tetrahedron_t = simplex_t<3>;

}


#endif

// end of file
