// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // the traits of the one-sided element, doubled across the two sides of the interface
    template <class baseTraitsT>
    struct InterfaceElementTraits {
        using mesh_cell_type = typename baseTraitsT::mesh_cell_type;
        using discretization_node_type = typename baseTraitsT::discretization_node_type;
        static constexpr int degree = baseTraitsT::degree;
        static constexpr int n_nodes = 2 * baseTraitsT::n_nodes;
    };

    // Class {InterfaceElement} is a butterfly element: the geometry of one interface cell and
    // the bulk degrees of freedom on either side of it, named after the interface tangent
    template <class baseElementT>
    class InterfaceElement {

      public:
        // the one-sided finite element this interface element is built from
        using base_element_type = baseElementT;
        // the traits of this element
        using traits = InterfaceElementTraits<typename base_element_type::traits>;
        // the parametric coordinates type
        using parametric_coordinates_type = typename base_element_type::parametric_coordinates_type;
        // the underlying mesh cell type
        using mesh_cell_type = typename base_element_type::mesh_cell_type;

        // the number of nodes on either side of the interface
        static constexpr int n_side_nodes = base_element_type::n_nodes;
        // the number of degrees of freedom coupled by this element (left side and right side)
        static constexpr int n_nodes = traits::n_nodes;

        // the connectivity of one side of the interface
        using side_connectivity_type = typename base_element_type::connectivity_type;
        // the connectivity of this element (the left side first, then the right side)
        using connectivity_type = std::array<typename traits::discretization_node_type, n_nodes>;

      public:
        // the constructor
        constexpr InterfaceElement(
            base_element_type && element, const side_connectivity_type & left,
            const side_connectivity_type & right) :
            _element(std::move(element)),
            _connectivity(_assemble_connectivity(left, right))
        {}

        // destructor
        constexpr ~InterfaceElement() = default;

        // default move constructor
        constexpr InterfaceElement(InterfaceElement &&) noexcept = default;

        // delete copy constructor
        constexpr InterfaceElement(const InterfaceElement &) = delete;

        // delete assignment operator
        constexpr InterfaceElement & operator=(const InterfaceElement &) = delete;

        // delete move assignment operator
        constexpr InterfaceElement & operator=(InterfaceElement &&) noexcept = delete;

      public:
        // get the degrees of freedom coupled by this element
        constexpr auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the parametrized element
        constexpr auto element() const noexcept { return _element.element(); }

        // get the element parametrization
        constexpr auto parametrization() const noexcept { return _element.parametrization(); }

        // get the mesh cell
        constexpr auto cell() const noexcept -> mesh_cell_type { return _element.cell(); }

        // get the one-sided shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_side_nodes)
        constexpr auto shape() const
        {
            return _element.template shape<a>();
        }

        // get the gradient of the one-sided shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_side_nodes)
        constexpr auto gradient() const
        {
            return _element.template gradient<a>();
        }

      private:
        // lay the two sides' degrees of freedom out in one connectivity
        static constexpr auto _assemble_connectivity(
            const side_connectivity_type & left,
            const side_connectivity_type & right) -> connectivity_type
        {
            connectivity_type connectivity;
            for (int a = 0; a < n_side_nodes; ++a) {
                connectivity[a] = left[a];
                connectivity[a + n_side_nodes] = right[a];
            }
            return connectivity;
        }

      private:
        // the one-sided finite element; not {const}, so that this element stays movable
        base_element_type _element;
        // the degrees of freedom on either side of the interface
        connectivity_type _connectivity;
    };

    // Class {InterfaceElements} is a collection of interface elements to assemble a term over;
    // it owns no degrees of freedom, it only references those of the bulk
    template <class interfaceElementT>
    class InterfaceElements {

      public:
        // the interface element type
        using element_type = interfaceElementT;
        // the type of discretization node the interface elements reference
        using discretization_node_type = typename element_type::traits::discretization_node_type;
        // the collection type
        using elements_type = std::vector<element_type>;

      public:
        // the constructor
        constexpr InterfaceElements(elements_type && elements) : _elements(std::move(elements)) {}

      public:
        // return an iterable collection of the interface elements
        constexpr auto elements() const noexcept -> const elements_type & { return _elements; }

      private:
        // the interface elements
        elements_type _elements;
    };

}    // namespace mito


// end of file
