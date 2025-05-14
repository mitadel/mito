
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
        using node_type = typename domain_type::node_type;
        using nodes_type = std::set<node_type>;

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
        auto domain() const -> const domain_type & { return _domain; }
        auto function() const -> const function_type & { return _function; }

      private:
        const domain_type & _domain;
        function_type _function;
    };

}    // namespace mito::constraints