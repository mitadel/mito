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
            _effective_load_vector("effective_load", semiDiscreteModel.n_equations()),
            _previous_solution("previous_solution", semiDiscreteModel.n_equations())
        {
            _previous_solution =
                _semi_discrete_model.template transform_field_to_vector<linalg_backend_type>(
                    initial_condition);
        }

        // assemble stiffness matrix
        constexpr auto assemble_stiffness_matrix() -> void
        {
            _semi_discrete_model.template assemble_matrix<stiffness_term_type, linalg_backend_type>(
                _stiffness_matrix);

            _stiffness_matrix.assemble();

            // all done
            return;
        }

        // assemble capacitance matrix
        constexpr auto assemble_capacitance_matrix() -> void
        {
            _semi_discrete_model
                .template assemble_matrix<capacitance_term_type, linalg_backend_type>(
                    _capacitance_matrix);

            _capacitance_matrix.assemble();

            // all done
            return;
        }

        // assemble load vector
        constexpr auto assemble_load_vector() -> void
        {
            _semi_discrete_model.template assemble_vector<load_term_type, linalg_backend_type>(
                _load_vector);

            _load_vector.assemble();

            // all done
            return;
        }

        // compute effective stiffness matrix
        constexpr auto compute_effective_stiffness_matrix(const mito::real & dt) -> void
        {
            // duplicate the stiffness matrix pattern
            _effective_stiffness_matrix.duplicate(_stiffness_matrix);

            // copy the stiffness matrix
            _effective_stiffness_matrix.copy_values(
                _stiffness_matrix, linalg::petsc::sparsity_pattern::same);

            // add the capacitance matrix scaled by 1/dt to the effective stiffness matrix
            _effective_stiffness_matrix.add(
                1. / dt, _capacitance_matrix, linalg::petsc::sparsity_pattern::same);

            // all done
            return;
        };

        // compute effective load vector
        constexpr auto compute_effective_load_vector(const mito::real & dt) -> void
        {
            // assemble the effective load vector
            _effective_load_vector.copy_values(_load_vector, linalg::petsc::sparsity_pattern::same);
            _effective_load_vector.add(1. / dt, _capacitance_matrix, _previous_solution);

            // all done
            return;
        }


      private:
        // the discrete system
        const semi_discrete_model_type & _semi_discrete_model;
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
        // the previous solution vector
        vector_type _previous_solution;
    };


}    // namespace mito::fem


// end of file
