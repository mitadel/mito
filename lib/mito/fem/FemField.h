// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {FemField} represents a finite element field defined via its nodal values on a set of
// discretization nodes. The field can be localized on finite elements via the {localize} method,
// which assembles the field values on the element from the nodal values and the element shape
// functions.

namespace mito::fem {

    // TODO: implement higher-dimensional fields (e.g. vector fields, tensor fields, ...)

    template <class fieldValueT>
    class FemField {

      private:
        // the field value type
        using field_value_type = fieldValueT;
        // the nodal field type
        using nodal_field_type = discrete::nodal_field_t<field_value_type>;
        // the node type
        using node_type = typename nodal_field_type::input_type;

      public:
        // constructor from temporary nodal field
        FemField(nodal_field_type && nodal_field) : _nodal_field(std::move(nodal_field)) {}

        // default destructor
        ~FemField() = default;

        // delete copy constructor
        FemField(const FemField &) = delete;

        // default move constructor
        FemField(FemField &&) = default;

        // delete copy assignment
        auto operator=(const FemField &) -> FemField & = delete;

        // delete move assignment
        auto operator=(FemField &&) -> FemField & = delete;

      public:
        // get a read-only nodal value at {node}
        auto operator()(const node_type & node) const -> const field_value_type &
        {
            return _nodal_field(node);
        }

        // get a mutable nodal value at {node}
        auto operator()(const node_type & node) -> field_value_type & { return _nodal_field(node); }

        // accessor to the underlying nodal field (read-only)
        auto nodal_values() const -> const nodal_field_type & { return _nodal_field; }

        // QUESTION: how do we guarantee that {element} is compatible with the function space that
        // built this field?
        // TODO: add concept that constrains {elementT} to be a finite element type
        // localize the function on {element}
        template <class elementT>
        auto localize(const elementT & element) const -> auto
        {
            // the element type
            using element_type = elementT;

            // helper lambda to assemble the solution on {element}
            constexpr auto _assemble = []<int... a>(
                                           const element_type & element,
                                           const nodal_field_type & field,
                                           tensor::integer_sequence<a...>) {
                // assemble the solution field from the shape functions
                return ((element.template shape<a>() * field(element.connectivity()[a])) + ...);
            };
            return _assemble(
                element, _nodal_field, tensor::make_integer_sequence<element_type::n_nodes>{});
        }

        // iterators on the nodal field
        constexpr auto begin() noexcept { return _nodal_field.begin(); }
        constexpr auto end() noexcept { return _nodal_field.end(); }
        constexpr auto begin() const noexcept { return _nodal_field.begin(); }
        constexpr auto end() const noexcept { return _nodal_field.end(); }

      private:
        // the field with the nodal values of the field
        nodal_field_type _nodal_field;
    };

}    // namespace mito


// end of file
