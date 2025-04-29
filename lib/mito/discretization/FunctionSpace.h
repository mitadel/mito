// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    template <manifolds::manifold_c manifoldT>
    class FunctionSpace {

      private:
        // the manifold type
        using manifold_type = manifoldT;
        // the mesh type
        using mesh_type = typename manifold_type::mesh_type;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the node type
        using node_type = typename cell_type::node_type;
        // the coordinate system type
        using coord_system_type = typename manifold_type::coordinate_system_type;
        // TOFIX: how do we inject this in the class?
        // the canonical simplex type
        static constexpr auto canonical_element = isoparametric_simplex<cell_type>();
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type = cell_type::barycentric_coordinates_type;
        // TOFIX
        using evaluated_shape_functions_type = std::map<node_type, mito::tensor::scalar_t>;
        using evaluated_shape_functions_gradients_type =
            std::map<node_type, mito::tensor::vector_t<2>>;

      public:
        // the default constructor
        constexpr FunctionSpace(const manifold_type & manifold) : _manifold(manifold) {};

        // destructor
        constexpr ~FunctionSpace() = default;

        // delete move constructor
        constexpr FunctionSpace(FunctionSpace &&) noexcept = delete;

        // delete copy constructor
        constexpr FunctionSpace(const FunctionSpace &) = delete;

        // delete assignment operator
        constexpr FunctionSpace & operator=(const FunctionSpace &) = delete;

        // delete move assignment operator
        constexpr FunctionSpace & operator=(FunctionSpace &&) noexcept = delete;

      public:
        // get all the shape functions of cell {cell} evaluated at the point {xi} in barycentric
        // coordinates
        auto shape(const cell_type & cell, const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_type
        {
            // get the shape functions of the canonical element
            auto shape_functions = canonical_element.shape(xi);

            // the nodes of the cell
            const auto & nodes = cell.nodes();

            // return the shape functions evaluated at {xi}
            return { { nodes[0], shape_functions[0] },
                     { nodes[1], shape_functions[1] },
                     { nodes[2], shape_functions[2] } };
        }

        // get all the shape functions gradients evaluated at the point {xi} in barycentric
        // coordinates
        auto gradient(const cell_type & cell, const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_gradients_type
        {
            // the nodes of the cell
            const auto & nodes = cell.nodes();

            // the origin of the coordinate system
            auto origin = typename coord_system_type::coordinates_type{};

            // the coordinates of the nodes of the triangle
            auto x_0 = _manifold.coordinates(nodes[0]) - origin;
            auto x_1 = _manifold.coordinates(nodes[1]) - origin;
            auto x_2 = _manifold.coordinates(nodes[2]) - origin;

            // the jacobian of the mapping from the reference element to the physical element
            // evaluated at {xi}
            auto J = canonical_element.jacobian(x_0, x_1, x_2, xi);

            // the derivative of the coordinates with respect to the barycentric coordinates
            auto J_inv = mito::tensor::inverse(J);

            // get the  shape functions derivatives at {xi}
            /*constexpr*/ auto dphi = canonical_element.gradient(xi);

            // return the spatial gradients of the shape functions evaluated at {xi}
            return { { nodes[0], dphi[0] * J_inv },
                     { nodes[1], dphi[1] * J_inv },
                     { nodes[2], dphi[2] * J_inv } };
        }

      private:
        // a const reference to the mesh
        const manifold_type & _manifold;
    };

}    // namespace mito


// end of file
