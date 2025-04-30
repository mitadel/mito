
// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::constraints {

    // class Dirichlet boundary condition
    template <class meshT, fields::field_c fieldT>
    class Dirichlet {
        // types
      public:
        using domain_type = meshT;
        using function_type = fieldT;
        using cells_type = domain_type::cells_type;

        // interface
      public:
        // constructor
        Dirichlet(const domain_type & domain, const function_type & function) :
            _domain(domain),
            _function(function)
        {}

        // destructor
        ~Dirichlet() = default;

        // accessors
        auto cells() const -> const cells_type & { return _domain.cells(); }
        auto function() const -> const function_type & { return _function; }

      private:
        const domain_type & _domain;
        function_type _function;
    };

}    // namespace mito::constraints