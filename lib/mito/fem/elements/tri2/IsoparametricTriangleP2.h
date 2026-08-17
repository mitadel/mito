// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP2} represents a 2-simplex (triangle) embedded in a D-dimensional
// physical space, equipped with quadratic shape functions defined in the parametric space.


namespace mito::fem {

    template <manifolds::parametrized_element_c parametrizedElementT>
    class IsoparametricTriangleP2 : public utilities::Invalidatable {

      public:
        // the underlying type of parametrized element
        using parametrized_element_type = parametrizedElementT;
        // the underlying mesh cell type
        using mesh_cell_type = typename parametrized_element_type::cell_type;
        // the metric volume form type
        using metric_volume_form_type = typename parametrized_element_type::metric_volume_form_type;
        // the coordinates type of the physical space
        using coordinates_type = typename metric_volume_form_type::input_type;
        // the dimension of the physical space
        static constexpr int dim = coordinates_type::dim;
        // the (euclidean) metric of the physical space in {coordinates_type} coordinates
        using ambient_metric_type = geometry::euclidean_metric<coordinates_type>;

        // the degree of the finite element
        static constexpr int degree = 2;
        // the traits of this element
        using traits = finite_element_family<mesh_cell_type, degree>;
        // the connectivity type of the element
        using connectivity_type = traits::connectivity_type;

        // the type of shape functions
        using shape_functions_type = ShapeTriangleP2;
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
        inline IsoparametricTriangleP2(
            const parametrized_element_type & element, const connectivity_type & connectivity) :
            _element(element),
            _connectivity(connectivity)
        {
            // check consistency between the number of nodes and the number of shape functions
            static_assert(n_nodes == traits::n_nodes);
        }

        // destructor
        inline ~IsoparametricTriangleP2() = default;

        // default move constructor
        constexpr IsoparametricTriangleP2(IsoparametricTriangleP2 &&) noexcept = default;

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

        // get the element parmetrization
        constexpr auto parametrization() const noexcept
        {
            // delegate to the underlying element
            return _element.parametrization();
        }

        // get the parmetrized element
        constexpr auto element() const noexcept
        {
            // all done
            return _element;
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
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<dim, 2> {
                    // store the coordinates of the vertices of the triangle in physical space
                    auto x0 = _element.parametrization()({ 0.0, 0.0 });
                    auto x1 = _element.parametrization()({ 1.0, 0.0 });
                    auto x2 = _element.parametrization()({ 0.0, 1.0 });
                    auto x3 = 0.5 * (x0 + x1);
                    auto x4 = 0.5 * (x1 + x2);
                    auto x5 = 0.5 * (x2 + x0);

                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();
                    constexpr auto dphi_3 = shape_functions.dshape<3>();
                    constexpr auto dphi_4 = shape_functions.dshape<4>();
                    constexpr auto dphi_5 = shape_functions.dshape<5>();

                    // compute the jacobian of the isoparametric mapping
                    return (
                        tensor::dyadic(x0, dphi_0(xi)) + tensor::dyadic(x1, dphi_1(xi))
                        + tensor::dyadic(x2, dphi_2(xi)) + tensor::dyadic(x3, dphi_3(xi))
                        + tensor::dyadic(x4, dphi_4(xi)) + tensor::dyadic(x5, dphi_5(xi)));
                });

            // and return it
            return jacobian_function;
        }

        // get the gradient of the a-th shape function as a function of parametric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // the pullback of the ambient metric along the element parametrization
            auto g =
                geometry::pullback_metric<ambient_metric_type>::field(_element.parametrization());
            // the intrinsic gradient of the a-th shape function on the parametric space
            auto dphi = operators::gradient(shape_functions.shape<a>(), g);
            // push the gradient forward to the physical space with the jacobian
            return jacobian() * dphi;
        }

      private:
        // the parametrized element (geometric information)
        const parametrized_element_type _element;
        // the finite element connectivity
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
