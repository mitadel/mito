// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP1} represents a second order simplex (triangle) living in 2D
// cartesian space, equipped with linear shape functions defined in the parametric space.


namespace mito::fem {

    template <class parametrizedElementT>
    class IsoparametricTriangleP1 : public utilities::Invalidatable {

      public:
        // the underlying type of parametrized element
        using element_type = parametrizedElementT;
        // the underlying mesh cell type
        using mesh_cell_type = typename element_type::cell_type;

        // the degree of the finite element
        static constexpr int degree = 1;
        // the traits of this element
        using traits = finite_element_family<mesh_cell_type, degree>;
        // the connectivity type of the element
        using connectivity_type = traits::connectivity_type;

        // the type of shape functions
        using shape_functions_type = ShapeTriangleP1;
        // the canonical element type
        using canonical_element_type = typename shape_functions_type::reference_element_type;
        // the parametric coordinates type
        using parametric_coordinates_type =
            typename canonical_element_type::parametric_coordinates_type;
        // the linear shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;

      public:
        // the default constructor
        inline IsoparametricTriangleP1(
            const element_type & element, const connectivity_type & connectivity) :
            _element(element),
            _connectivity(connectivity)
        {
            // check consistency between the number of nodes and the number of shape functions
            static_assert(n_nodes == traits::n_nodes);
        }

        // destructor
        inline ~IsoparametricTriangleP1() = default;

        // default move constructor
        constexpr IsoparametricTriangleP1(IsoparametricTriangleP1 &&) noexcept = default;

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

        // get the element parmetrization
        constexpr auto parametrization() const noexcept
        {
            // delegate to the underlying element
            return _element.parametrization();
        }

        // get the mesh cell
        constexpr auto cell() const noexcept -> mesh_cell_type { return _element.cell(); }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            // return the shape functions
            return shape_functions.shape<a>();
        }

        // get the jacobian of the isoparametric mapping from parametric to actual coordinates
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of parametric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<2> {
                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();

                    // store the coordinates of the vertices of the triangle in physical space
                    auto x0 = _element.parametrization()({ 0.0, 0.0 });
                    auto x1 = _element.parametrization()({ 1.0, 0.0 });
                    auto x2 = _element.parametrization()({ 0.0, 1.0 });

                    // compute the jacobian of the isoparametric mapping
                    return (
                        tensor::dyadic(x0, dphi_0(xi)) + tensor::dyadic(x1, dphi_1(xi))
                        + tensor::dyadic(x2, dphi_2(xi)));
                });

            // and return it
            return jacobian_function;
        }

        // get the gradient of the a-th shape function as a function of parametric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // assemble the gradient as a function of parametric coordinates
            auto gradient_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::vector_t<2> {
                    // the jacobian of the mapping from the reference element to the physical
                    // element evaluated at {xi}
                    auto J = jacobian()(xi);
                    // the derivative of the coordinates with respect to the parametric coordinates
                    auto J_inv = tensor::inverse(J);
                    // return the spatial gradients of the shape functions evaluated at {xi}
                    return shape_functions.dshape<a>()(xi) * J_inv;
                });

            // and return it
            return gradient_function;
        }

      private:
        // the parametrized element (geometric information)
        const element_type _element;
        // the finite element connectivity
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
