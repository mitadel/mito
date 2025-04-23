// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    class FunctionSpace {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // TOFIX: how do we inject this in the class?
        // the canonical simplex type
        static constexpr auto canonical_element = isoparametric_simplex<cell_type>();
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type = cell_type::barycentric_coordinates_type;
        // TOFIX
        using evaluated_shape_functions_type = std::array<mito::tensor::scalar_t, 3>;
        using evaluated_shape_functions_gradients_type = std::array<mito::tensor::vector_t<2>, 3>;

      public:
        // the default constructor
        constexpr FunctionSpace(const mesh_type & mesh, const coord_system_type & coord_system) :
            _mesh(mesh),
            _coord_system(coord_system) {};

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
        // TOFIX: {barycentricCoordinatesT} should be read from the cell type traits
        // get all the shape functions evaluated at the point {xi} in barycentric coordinates
        auto shape(/*const cell_type & cell,*/ const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_type
        {
            // return the shape functions evaluated at {xi}
            return canonical_element.shape(xi);
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
            auto x_0 = _coord_system.coordinates(nodes[0]->point()) - origin;
            auto x_1 = _coord_system.coordinates(nodes[1]->point()) - origin;
            auto x_2 = _coord_system.coordinates(nodes[2]->point()) - origin;

            // the jacobian of the mapping from the reference element to the physical element
            // evaluated at {xi}
            auto J = canonical_element.jacobian(x_0, x_1, x_2, xi);

            // the derivative of the coordinates with respect to the barycentric coordinates
            auto J_inv = mito::tensor::inverse(J);

            // get the  shape functions derivatives at {xi}
            /*constexpr*/ auto dphi = canonical_element.gradient(xi);

            // return the spatial gradients of the shape functions evaluated at {xi}
            return { dphi[0] * J_inv, dphi[1] * J_inv, dphi[2] * J_inv };
        }

      private:
        // TOFIX: this is unused for now
        // a const reference to the mesh
        const mesh_type & _mesh;

        // a const reference to the coordinate system
        const coord_system_type & _coord_system;
    };

}    // namespace mito


// end of file
