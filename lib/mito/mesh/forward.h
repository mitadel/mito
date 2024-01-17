// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <template <int> class cellT, int D>
    class Mesh;

    // mesh alias
    template <template <int> class cellT, int D>
    using mesh_t = Mesh<cellT, D>;
}


#endif

// end of file
