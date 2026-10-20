// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <class semiDiscreteModelT, linalg::valid_backend_c linalgBackendT>
    class ImplicitEuler {

      private:
        // the semi discrete model type
        using semi_discrete_model_type = semiDiscreteModelT;
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the archetype type
        using archetype_type = typename semi_discrete_model_type::archetype_type;
        // the stiffness term type
        using stiffness_term_type = typename archetype_type::Stiffness;
        // the capacitance term type
        using capacitance_term_type = typename archetype_type::Capacitance;
        // the load term type
        using load_term_type = typename archetype_type::Load;

      public:
        // the default constructor
        constexpr ImplicitEuler(const semi_discrete_model_type & semiDiscreteModel) :
            _semiDiscreteModel(semiDiscreteModel),
            _stiffness_matrix("stiffness", semiDiscreteModel.n_equations()),
            _capacitance_matrix("capacitance", semiDiscreteModel.n_equations()),
            _load_vector("load", semiDiscreteModel.n_equations()),
            _effective_stiffness_matrix("effective_stiffness", semiDiscreteModel.n_equations()),
            _effective_load_vector("effective_load", semiDiscreteModel.n_equations())
        {
            ;
        }

        // assemble stiffness matrix
        constexpr auto assemble_stiffness_matrix() -> void
        {
            _semiDiscreteModel.template assemble_matrix<stiffness_term_type, linalg_backend_type>(
                _stiffness_matrix);

            _stiffness_matrix.assemble();
        }

        // assemble capacitance matrix
        constexpr auto assemble_capacitance_matrix() -> void
        {
            _semiDiscreteModel.template assemble_matrix<capacitance_term_type, linalg_backend_type>(
                _capacitance_matrix);

            _capacitance_matrix.assemble();
        }

        // assemble load vector
        constexpr auto assemble_load_vector() -> void
        {
            _semiDiscreteModel.template assemble_vector<load_term_type, linalg_backend_type>(
                _load_vector);

            _load_vector.assemble();
        }

        // assemble effective stiffness matrix
        constexpr auto assemble_effective_stiffness_matrix(const mito::real & dt) -> void {
            // assemble the effective stiffness matrix
            // _stiffness_matrix.assemble();
            // _capacitance_matrix.assemble();
            // _stiffness_matrix.add_matrix(_capacitance_matrix, 1.0 / dt);
            // _stiffness_matrix.assemble_flush();
        };

      private:
        // the discrete system
        const semi_discrete_model_type & _semiDiscreteModel;
        // the stiffness matrix
        matrix_type _stiffness_matrix;
        // the capacitance matrix
        matrix_type _capacitance_matrix;
        // the load vector
        vector_type _load_vector;
        // the effective stiffness matrix
        matrix_type _effective_stiffness_matrix;
        // the effective load vector
        vector_type _effective_load_vector;
    };


}    // namespace mito::fem


// end of file
