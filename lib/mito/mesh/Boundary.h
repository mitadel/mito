// code guard
#pragma once


namespace mito::mesh {

    // type for a boundary mesh of a mesh of cells {cellT}
    template <class cellT>
    struct boundary_mesh;

    // type for a boundary mesh of a mesh of cells {cellT}
    template <geometry::geometric_simplex_c cellT>
    struct boundary_mesh<cellT> {
        // typedef my template parameter
        using cell_type = cellT;
        // the dimension of physical space
        static constexpr int D = cell_type::dim;
        // get the order of the cell
        static constexpr int N = cell_type::order;
        // get the family this cell type belongs to (e.g. geometric simplices)
        template <int I>
        using cell_family_type = typename cell_type::cell_family_type<I, D>;
        // the boundary type: a boundary mesh
        using type = mesh_t<cell_family_type<N - 1>>;
    };

    // specialization for geometric segments
    template <geometry::geometric_segment_c cellT>
    struct boundary_mesh<cellT> {
        // typedef my template parameter
        using cell_type = cellT;
        // the node type
        using node_type = cell_type::node_type;
        // the boundary type: a cloud of nodes
        using type = std::unordered_set<node_type, utilities::hash_function<node_type>>;
    };

    template <class meshT>
    class Boundary {

      private:
        // typedef my template parameter
        using mesh_type = meshT;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the dimension of physical space
        static constexpr int D = cell_type::dim;
        // boundary type: either a mesh or a collection of nodes (boundary of a mesh of segments)
        using boundary_mesh_type = typename boundary_mesh<cell_type>::type;

      public:
        // returns the boundary of {mesh}
        static inline auto boundary(const mesh_type & mesh) -> boundary_mesh_type;

        // returns the size of the boundary of {mesh}
        static inline auto boundary_size(const mesh_type & mesh) -> int;
    };
}


// get the inline definitions
#define mito_mesh_boundary_icc
#include "Boundary.icc"
#undef mito_mesh_boundary_icc


// end of file
