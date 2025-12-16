// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {DomainField} represents a field defined over a domain. It is built from a continuous
// field defined in the physical space. The field can be localized on finite elements via the
// {localize} method, which composes the underlying field with the element parametrization.


namespace mito::fem {

    template <fields::field_c F>
    class DomainField {

      private:
        // the underlying field type
        using field_type = F;

      public:
        // default constructor
        constexpr DomainField(const F & field) : _field(field) {}

        // default destructor
        constexpr ~DomainField() = default;

        // delete copy constructor
        constexpr DomainField(const DomainField &) = delete;

        // default move constructor
        constexpr DomainField(DomainField &&) = delete;

        // delete copy assignment
        constexpr auto operator=(const DomainField &) -> DomainField & = delete;

        // delete move assignment
        constexpr auto operator=(DomainField &&) -> DomainField & = delete;

      public:
        // localize the field on {element}
        template <class elementT>
        constexpr auto localize(const elementT & element) const -> auto
        {
            // return the composition of the underlying field with the element parametrization
            return _field(element.parametrization());
        }

      private:
        // the underlying global field
        const field_type _field;
    };

}    // namespace mito


// end of file
