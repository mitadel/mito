// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP1} represents a 2-simplex (triangle) embedded in a D-dimensional
// physical space, equipped with linear shape functions defined in the parametric space.


namespace mito::fem {

    template <manifolds::parametrized_element_c parametrizedElementT>
    class IsoparametricTriangleP1 : public utilities::Invalidatable {

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
            const parametrized_element_type & element, const connectivity_type & connectivity) :
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

        // get the parmetrized element
        constexpr auto element() const noexcept
        {
            // all done
            return _element;
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
            return _element.jacobian() * dphi;
        }

      private:
        // the parametrized element (geometric information)
        const parametrized_element_type _element;
        // the finite element connectivity
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
