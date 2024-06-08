// code guard
#pragma once


namespace mito::mesh {

    template <class meshT>
    class Boundary {

      private:
        // typedef my template parameter
        using mesh_type = meshT;
        // typedef cell type
        using cell_type = typename mesh_type::cell_type;
        // the dimension of physical space
        static constexpr int D = cell_type::dim;
        // get the order of the cell
        static constexpr int N = cell_type::order;
        // get the family this cell type belongs to (e.g. geometric simplices)
        template <int I>
        using cell_family_type = typename cell_type::cell_family_type<I, D>;
        // the boundary mesh type
        using mesh_boundary_type = mesh_t<cell_family_type<N - 1>>;

      public:
        // returns the boundary mesh of {mesh}
        static inline auto boundary(const mesh_type & mesh) -> mesh_boundary_type
        requires(N > 0);

        // returns the size of the boundary of {mesh}
        static inline auto boundary_size(const mesh_type & mesh) -> int
        requires(N > 0);
    };
}


// get the inline definitions
#define mito_mesh_boundary_icc
#include "Boundary.icc"
#undef mito_mesh_boundary_icc


// end of file
