// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // a linear first-order problem M du/dt + K u = f, described by a semi-discrete model; the
    // linear algebra backend is chosen by the caller, per call
    template <class semiDiscreteModelT>
    requires(std::same_as<typename semiDiscreteModelT::archetype_type, models::ParabolicPDE>)
    class LinearFirstOrderProblem {

      private:
        // the semi discrete model type
        using semi_discrete_model_type = semiDiscreteModelT;
        // the archetype type
        using archetype_type = typename semiDiscreteModelT::archetype_type;

      public:
        // time invariance flag
        static constexpr bool time_invariant = true;
        // constant load flag
        static constexpr bool constant_load = true;

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

        // transform a field to a vector, usable with the backend {backendT}
        template <linalg::valid_backend_c backendT>
        auto transform_field_to_vector(const fields::field_c auto & field) const ->
            typename backendT::vector_type
        {
            return _model.template transform_field_to_vector<backendT>(field);
        }

        // transform a vector to a field, usable with the backend {backendT}
        template <linalg::valid_backend_c backendT>
        auto transform_vector_to_field(const typename backendT::vector_type & vector) const ->
            typename semi_discrete_model_type::solution_field_type
        {
            return _model.template transform_vector_to_field<backendT>(vector);
        }

      private:
        // the semi-discrete model
        const semi_discrete_model_type & _model;
    };

}    // namespace mito::fem


// end of file
