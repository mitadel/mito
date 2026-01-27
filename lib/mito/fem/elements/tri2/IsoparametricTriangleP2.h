// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP2} represents a second order simplex equipped with quadratic shape
// functions defined in the parametric space. This unified implementation works for:
//   - Triangles in any coordinate system (Cartesian, polar, spherical, etc.)
//   - Triangles in any embedding (2D in 2D, 2D in 3D, etc.)


namespace mito::fem {

    template <geometry::coordinates_c coordsT, class VolumeFormT>
    class IsoparametricTriangleP2 : public IsoparametricTriangle<coordsT, VolumeFormT> {

      private:
        using base_type = IsoparametricTriangle<coordsT, VolumeFormT>;

      public:
        static constexpr int D = base_type::D;    // ambient dimension
        static constexpr int N =
            base_type::N;                // dimension of the parametric space (=2 for triangle)
        static constexpr int dim = D;    // expected by FunctionSpace

        // the degree of the finite element
        static constexpr int degree = 2;
        // the type of shape functions
        using shape_functions_type = ShapeTriangleP2;
        // the canonical element type
        using canonical_element_type = typename shape_functions_type::reference_element_type;
        // type of a point in parametric coordinates
        using parametric_coordinates_type =
            typename canonical_element_type::parametric_coordinates_type;
        // the quadratic shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;
        // a collection of discretization nodes
        using connectivity_type = std::array<typename base_type::discretization_node_type, n_nodes>;

        // import types from base
        using typename base_type::cell_type;
        using typename base_type::coordinate_system_type;
        using typename base_type::coordinates_type;
        using typename base_type::vector_type;
        using typename base_type::volume_form_type;

      public:
        // the default constructor
        inline IsoparametricTriangleP2(
            const cell_type & geometric_simplex, const coordinate_system_type & coord_system,
            const connectivity_type & connectivity, const volume_form_type & volume_form) :
            base_type(geometric_simplex, coord_system, volume_form),
            _connectivity(connectivity)
        {}

        // destructor
        ~IsoparametricTriangleP2() = default;

        // delete move constructor
        constexpr IsoparametricTriangleP2(IsoparametricTriangleP2 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangleP2(const IsoparametricTriangleP2 &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangleP2 & operator=(const IsoparametricTriangleP2 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangleP2 & operator=(IsoparametricTriangleP2 &&) noexcept = delete;

      public:
        // get the discretization nodes
        constexpr auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            // return the shape functions
            return shape_functions.shape<a>();
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<2> {
                    auto x3 = 0.5 * (_x0 + _x1);
                    auto x4 = 0.5 * (_x1 + _x2);
                    auto x5 = 0.5 * (_x2 + _x0);

                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();
                    constexpr auto dphi_3 = shape_functions.dshape<3>();
                    constexpr auto dphi_4 = shape_functions.dshape<4>();
                    constexpr auto dphi_5 = shape_functions.dshape<5>();

                    // compute the gradient of the isoparametric mapping
                    return (
                        tensor::dyadic(_x0, dphi_0(xi)) + tensor::dyadic(_x1, dphi_1(xi))
                        + tensor::dyadic(_x2, dphi_2(xi)) + tensor::dyadic(x3, dphi_3(xi))
                        + tensor::dyadic(x4, dphi_4(xi)) + tensor::dyadic(x5, dphi_5(xi)));
                });

            // and return it
            return jacobian_function;
        }

        // volume element: contract the volume form with the two tangent vectors
        constexpr auto volume_element() const
        {
            return functions::function([&](const parametric_coordinates_type & xi) {
                // physical point
                auto x = this->parametrization()(xi);

                // get the two tangent vectors from the Jacobian
                auto J = jacobian()(xi);

                // extract tangent vectors from the D×2 matrix
                auto tangent_0 = [&J]() {
                    if constexpr (D == 2) {
                        return tensor::vector_t<2>{ J[{ 0, 0 }], J[{ 1, 0 }] };
                    } else if constexpr (D == 3) {
                        return tensor::vector_t<3>{ J[{ 0, 0 }], J[{ 1, 0 }], J[{ 2, 0 }] };
                    }
                }();

                auto tangent_1 = [&J]() {
                    if constexpr (D == 2) {
                        return tensor::vector_t<2>{ J[{ 0, 1 }], J[{ 1, 1 }] };
                    } else if constexpr (D == 3) {
                        return tensor::vector_t<3>{ J[{ 0, 1 }], J[{ 1, 1 }], J[{ 2, 1 }] };
                    }
                }();

                // contract the volume form with both tangent vectors
                // for N=2, include factorial (1/2!)
                return (1.0 / 2.0) * this->_volume_form(x)(tangent_0, tangent_1);
            });
        }

        // get the gradient of the a-th shape function as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // assemble the gradient as a function of barycentric coordinates
            auto gradient_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::vector_t<2> {
                    // the jacobian of the mapping from the reference element to the physical
                    // element evaluated at {xi}
                    auto J = jacobian()(xi);
                    // the derivative of the coordinates with respect to the barycentric coordinates
                    auto J_inv = tensor::inverse(J);
                    // return the spatial gradients of the shape functions evaluated at {xi}
                    return shape_functions.dshape<a>()(xi) * J_inv;
                });
            // and return it
            return gradient_function;
        }

      private:
        // the discretization nodes of the simplex
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
