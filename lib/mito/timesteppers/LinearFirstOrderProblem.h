// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // a linear first-order problem M du/dt + K u = f, described by a semi-discrete model; the
    // linear algebra backend is chosen by the caller, per call
    template <class semiDiscreteModelT>
    requires parabolic_archetype_c<typename semiDiscreteModelT::archetype_type>
    class LinearFirstOrderProblem {

      private:
        // the semi discrete model type
        using semi_discrete_model_type = semiDiscreteModelT;
        // the archetype type
        using archetype_type = typename semiDiscreteModelT::archetype_type;

      public:
        // constructor
        constexpr explicit LinearFirstOrderProblem(const semi_discrete_model_type & model) :
            _model(model)
        {}

        // the number of equations
        constexpr auto n_equations() const -> int { return _model.n_equations(); }

        // set the sparsity pattern of a system matrix
        template <linalg::valid_backend_c backendT>
        auto set_pattern(typename backendT::matrix_type & A) const -> void
        {
            _model.template set_matrix_pattern<backendT>(A);
        }

        // the mass matrix M: the capacitance term of the archetype
        template <linalg::valid_backend_c backendT>
        auto assemble_mass(typename backendT::matrix_type & M) const -> void
        {
            _model.template set_matrix_values<typename archetype_type::Capacitance, backendT>(M);
            M.assemble();
        }

        // the stiffness matrix K: the stiffness term of the archetype
        template <linalg::valid_backend_c backendT>
        auto assemble_stiffness(typename backendT::matrix_type & K) const -> void
        {
            _model.template set_matrix_values<typename archetype_type::Stiffness, backendT>(K);
            K.assemble();
        }

        // the load vector f: the load term of the archetype
        template <linalg::valid_backend_c backendT>
        auto assemble_load(typename backendT::vector_type & f) const -> void
        {
            _model.template set_vector_values<typename archetype_type::Load, backendT>(f);
            f.assemble();
        }

      private:
        // the semi-discrete model
        const semi_discrete_model_type & _model;
    };

}    // namespace mito::timesteppers


// end of file
