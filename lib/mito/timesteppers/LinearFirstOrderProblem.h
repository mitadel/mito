// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // A class representing a linear first-order problem
    template <class semiDiscreteModelT, linalg::valid_backend_c linalgBackendT>
    requires parabolic_archetype_c<typename semiDiscreteModelT::archetype_type>
    class LinearFirstOrderProblem {

      private:
        // the semi discrete model type
        using semi_discrete_model_type = semiDiscreteModelT;
        // the archetype type
        using archetype_type = typename semiDiscreteModelT::archetype_type;

      public:
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;

      public:
        // the default constructor
        constexpr LinearFirstOrderProblem(const semiDiscreteModelT & model) : _model(model) {}

        // the number of equations
        constexpr auto n_equations() const -> int { return _model.n_equations(); }

        // set the sparsity pattern of a system matrix
        auto set_pattern(matrix_type & A) const -> void
        {
            _model.template set_matrix_pattern<backendT>(A);
        }

        // the mass matrix M: the capacitance term of the archetype
        auto assemble_mass(matrix_type & M) const -> void
        {
            _model.template set_matrix_values<typename archetype_type::Capacitance, backendT>(M);
            M.assemble();
        }

        // the stiffness matrix K: the stiffness term of the archetype
        auto assemble_stiffness(matrix_type & K) const -> void
        {
            _model.template set_matrix_values<typename archetype_type::Stiffness, backendT>(K);
            K.assemble();
        }

        // the load vector f: the load term of the archetype
        auto assemble_load(vector_type & f) const -> void
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
