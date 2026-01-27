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
        using metric_space_type = typename base_type::metric_space_type;

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
        // returns a D×2 matrix (columns are the two tangent/director vectors)
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<D, 2> {
                    auto x3 = 0.5 * (this->_x0 + this->_x1);
                    auto x4 = 0.5 * (this->_x1 + this->_x2);
                    auto x5 = 0.5 * (this->_x2 + this->_x0);

                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();
                    constexpr auto dphi_3 = shape_functions.dshape<3>();
                    constexpr auto dphi_4 = shape_functions.dshape<4>();
                    constexpr auto dphi_5 = shape_functions.dshape<5>();

                    // compute the gradient of the isoparametric mapping
                    return (
                        tensor::dyadic(this->_x0, dphi_0(xi))
                        + tensor::dyadic(this->_x1, dphi_1(xi))
                        + tensor::dyadic(this->_x2, dphi_2(xi)) + tensor::dyadic(x3, dphi_3(xi))
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
                auto tangent_0 = pyre::tensor::col<0>(J);
                auto tangent_1 = pyre::tensor::col<1>(J);

                // contract the volume form with both tangent vectors
                // for N=2, include factorial (1/2!)
                return (1.0 / 2.0) * this->_volume_form(x)(tangent_0, tangent_1);
            });
        }

        // get the gradient of the a-th shape function as a function of barycentric coordinates
        // returns a contravariant vector (raised index)
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // assemble the gradient as a function of barycentric coordinates
            auto gradient_function =
                functions::function([&](const parametric_coordinates_type & xi) -> vector_type {
                    // physical point
                    auto x = this->parametrization()(xi);

                    // jacobian matrix (D×2 for surfaces)
                    auto J = jacobian()(xi);

                    // metric tensors at the physical point
                    auto g = base_type::g(x);
                    auto g_inv = base_type::g_inv(x);

                    // parametric derivative (2D vector for N=2)
                    constexpr auto dphi_dxi = shape_functions.dshape<a>();
                    auto dphi_dxi_val = dphi_dxi(xi);

                    // compute gradient (handles both square and embedded cases)
                    return fem::compute_gradient<D, N>(J, g, g_inv, dphi_dxi_val);
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
