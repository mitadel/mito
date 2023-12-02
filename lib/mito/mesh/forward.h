// code guard
#if !defined(mito_mesh_forward_h)
#define mito_mesh_forward_h


namespace mito::mesh {

    // class mesh
    template <class cellT, int D, geometry::CoordinateSystem coordT>
    class Mesh;

    // mesh alias
    template <class cellT, int D, geometry::CoordinateSystem coordT>
    using mesh_t = Mesh<cellT, D, coordT>;
}


#endif

// end of file
