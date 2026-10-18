// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // factory of finite element from a parametrized element
    template <class finiteElementTraits, class parametrizedElementT>
    requires compatible_element_type_c<parametrizedElementT, finiteElementTraits>
    constexpr auto finite_element(
        const parametrizedElementT & element,
        const typename finiteElementTraits::connectivity_type & connectivity)
    {
        // get the type of the finite element from the traits
        using finite_element_type =
            typename finiteElementTraits::template type<parametrizedElementT>;

        // assemble the finite element from the parametrized element and the connectivity
        return finite_element_type(element, connectivity);
    }

    // interface elements factory: one butterfly element per cell of {manifold}, coupling the
    // degrees of freedom that {left_node_map} and {right_node_map} assign to its nodes. The same
    // map twice identifies the two sides, which is the continuous case
    template <class finiteElementTraitsT, class manifoldT, class leftMapT, class rightMapT>
    auto interface_elements(
        const manifoldT & manifold, const leftMapT & left_node_map,
        const rightMapT & right_node_map)
    {
        // the cell type of the interface mesh
        using cell_type = typename manifoldT::mesh_type::cell_type;
        // the type of parametrized element the manifold hands out
        using parametrized_element_type =
            std::remove_cvref_t<decltype(manifold.element(std::declval<const cell_type &>()))>;
        // the one-sided finite element type
        using base_element_type =
            typename finiteElementTraitsT::template type<parametrized_element_type>;
        // the interface element type
        using interface_element_type = InterfaceElement<base_element_type>;
        // the connectivity of one side of the interface
        using side_connectivity_type = typename finiteElementTraitsT::connectivity_type;
        // the number of nodes on either side of the interface
        constexpr int n_side_nodes = finiteElementTraitsT::n_nodes;

        // reserve room for one interface element per cell
        auto elements = std::vector<interface_element_type>();
        elements.reserve(manifold.mesh().nCells());

        // loop on the cells of the interface mesh
        for (const auto & cell : manifold.mesh().cells()) {
            // get the mesh nodes of this cell
            const auto & nodes = cell.nodes();
            // look up the degrees of freedom that either side assigns to them
            auto left = side_connectivity_type();
            auto right = side_connectivity_type();
            for (int a = 0; a < n_side_nodes; ++a) {
                left[a] = left_node_map.at(nodes[a]);
                right[a] = right_node_map.at(nodes[a]);
            }
            // assemble the interface element on this cell
            elements.emplace_back(
                finite_element<finiteElementTraitsT>(manifold.element(cell), left), left, right);
        }

        // all done
        return InterfaceElements<interface_element_type>(std::move(elements));
    }

}


// end of file
