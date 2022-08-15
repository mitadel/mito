// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D>
    Mesh<D> summit(std::ifstream & filename)
    {
        Mesh<D> summit_mesh("some string");
        return summit_mesh;
    }
}    // namespace mito::mesh

#endif    // mito_mesh_summit_h