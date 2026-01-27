// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP1} represents a second order simplex equipped with linear shape
// functions defined in the parametric space. This unified implementation works for:
//   - Triangles in any coordinate system (Cartesian, polar, spherical, etc.)
//   - Triangles in any embedding (2D in 2D, 2D in 3D, etc.)


namespace mito::fem {

    template <geometry::coordinates_c coordsT, class VolumeFormT>
    class IsoparametricTriangleP1 : public IsoparametricTriangle<coordsT, VolumeFormT> {

      private:
        using base_type = IsoparametricTriangle<coordsT, VolumeFormT>;

      public:
        static constexpr int D = base_type::D;    // ambient dimension
        static constexpr int N =
            base_type::N;                // dimension of the parametric space (=2 for triangle)
        static constexpr int dim = D;    // expected by FunctionSpace

        // the degree of the finite element
        static constexpr int degree = 1;
        // the type of shape functions
        using shape_functions_type = ShapeTriangleP1;
        // the canonical element type
        using canonical_element_type = typename shape_functions_type::reference_element_type;
        // the parametric coordinates type
        using parametric_coordinates_type =
            typename canonical_element_type::parametric_coordinates_type;
        // the linear shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;
        // a collection of discretization discretization nodes
        using connectivity_type = std::array<typename base_type::discretization_node_type, n_nodes>;

        // import types from base
        using typename base_type::cell_type;
        using typename base_type::coordinate_system_type;
        using typename base_type::coordinates_type;
        using typename base_type::vector_type;
        using typename base_type::volume_form_type;

      public:
        // the default constructor
        inline IsoparametricTriangleP1(
            const cell_type & geometric_simplex, const coordinate_system_type & coord_system,
            const connectivity_type & connectivity, const volume_form_type & volume_form) :
            base_type(geometric_simplex, coord_system, volume_form),
            _connectivity(connectivity)
        {}

        // destructor
        inline ~IsoparametricTriangleP1() = default;

        // delete move constructor
        constexpr IsoparametricTriangleP1(IsoparametricTriangleP1 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangleP1(const IsoparametricTriangleP1 &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangleP1 & operator=(const IsoparametricTriangleP1 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangleP1 & operator=(IsoparametricTriangleP1 &&) noexcept = delete;

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
        // returns a D×2 matrix (columns are the two tangent/director vectors)
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<D, 2> {
                    // get the shape functions derivatives (these are 2D vectors in parametric
                    // space)
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();

                    // compute the gradient of the isoparametric mapping
                    // this is the outer product of position vectors with gradient vectors
                    return (
                        tensor::dyadic(this->_x0, dphi_0(xi))
                        + tensor::dyadic(this->_x1, dphi_1(xi))
                        + tensor::dyadic(this->_x2, dphi_2(xi)));
                });

            // and return it
            return jacobian_function;
        }

        // volume element: contract the volume form with the two tangent vectors
        // this follows the same pattern as Manifold::_volume
        constexpr auto volume_element() const
        {
            return functions::function([&](const parametric_coordinates_type & xi) {
                // physical point
                auto x = this->parametrization()(xi);

                // get the two tangent vectors from the Jacobian
                auto J = jacobian()(xi);

                // extract tangent vectors from the D×2 matrix
                auto tangent_0 = pyre::tensor::col<0>(J);
                auto tangent_1 = pyre::tensor::col<1>(J);

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
