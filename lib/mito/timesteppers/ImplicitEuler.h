// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    class ImplicitEuler {

      private:
        // the semi discrete model type
        using semi_discrete_model_type = semiDiscreteModelT;
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the matrix solver type
        using solver_type = solverT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the scalar type
        using scalar_type = typename linalg_backend_type::scalar_type;
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
        constexpr ImplicitEuler(
            const semi_discrete_model_type & semi_discrete_model, solver_type && solver,
            const fields::field_c auto & initial_condition, scalar_type dt) :
            _semi_discrete_model(semi_discrete_model),
            _solver(std::move(solver)),
            _dt(dt),
            _stiffness_matrix("stiffness", _semi_discrete_model.n_equations()),
            _capacitance_matrix("capacitance", _semi_discrete_model.n_equations()),
            _load_vector("load", _semi_discrete_model.n_equations()),
            _effective_stiffness_matrix("effective_stiffness", _semi_discrete_model.n_equations()),
            _effective_load_vector("effective_load", _semi_discrete_model.n_equations()),
            _temp_vector("temp", _semi_discrete_model.n_equations()),
            _solution(
                "solution",
                _semi_discrete_model.template transform_field_to_vector<linalg_backend_type>(
                    initial_condition))
        {
            ;
        }

        constexpr auto initialise() -> void
        {
            // set the patterns
            _semi_discrete_model.template set_matrix_pattern<linalg_backend_type>(
                _stiffness_matrix);
            _semi_discrete_model.template set_matrix_pattern<linalg_backend_type>(
                _capacitance_matrix);

            // set the values
            assemble_stiffness_matrix();
            assemble_capacitance_matrix();
            assemble_load_vector();

            // initialise the effective stiffness matrix and load vector
            initialise_effective_stiffness_matrix();

            // compute the effective stiffness matrix and load vector
            compute_effective_stiffness_matrix();
            compute_effective_load_vector();
        }

        // assemble stiffness matrix
        constexpr auto assemble_stiffness_matrix() -> void
        {
            _semi_discrete_model
                .template set_matrix_values<stiffness_term_type, linalg_backend_type>(
                    _stiffness_matrix);

            _stiffness_matrix.assemble();

            // all done
            return;
        }

        // assemble capacitance matrix
        constexpr auto assemble_capacitance_matrix() -> void
        {
            _semi_discrete_model
                .template set_matrix_values<capacitance_term_type, linalg_backend_type>(
                    _capacitance_matrix);

            _capacitance_matrix.assemble();

            // all done
            return;
        }

        // assemble load vector
        constexpr auto assemble_load_vector() -> void
        {
            _semi_discrete_model.template set_vector_values<load_term_type, linalg_backend_type>(
                _load_vector);

            _load_vector.assemble();

            // all done
            return;
        }

        // initialise effective stiffness matrix
        constexpr auto initialise_effective_stiffness_matrix() -> void
        {
            // duplicate the stiffness matrix pattern
            _effective_stiffness_matrix.duplicate(_stiffness_matrix);

            // all done
            return;
        }

        // compute effective stiffness matrix
        constexpr auto compute_effective_stiffness_matrix() -> void
        {
            // copy the stiffness matrix
            _effective_stiffness_matrix.copy_values(
                _stiffness_matrix, linalg::petsc::sparsity_pattern::same);

            // add the capacitance matrix scaled by 1/dt to the effective stiffness matrix
            _effective_stiffness_matrix.add(
                1. / _dt, _capacitance_matrix, linalg::petsc::sparsity_pattern::same);

            // all done
            return;
        };

        // compute effective load vector
        constexpr auto compute_effective_load_vector() -> void
        {
            // compute M * u_n and store it in the temporary vector
            _capacitance_matrix.mult_vector(_solution, _temp_vector);

            // initialize the effective load vector with the load vector
            _effective_load_vector.copy_values(_load_vector);

            // add the temporary vector scaled by 1/dt to the effective load vector
            _effective_load_vector.add(1. / _dt, _temp_vector);

            // all done
            return;
        }

        // set the time step size
        constexpr auto set_dt(scalar_type dt) -> void
        {
            // set the time step size
            _dt = dt;

            // reassemble the effective stiffness matrix with the new time step size
            compute_effective_stiffness_matrix();

            // all done
            return;
        }

        // march one time step
        constexpr auto march() -> void
        {
            // compute the effective load vector
            compute_effective_load_vector();

            // solve the linear system
            _solver.solve(_effective_stiffness_matrix, _effective_load_vector, _solution);

            // all done
            return;
        }

        // get the solution as a finite element field
        constexpr auto solution_field() const -> semi_discrete_model_type::fem_field_type
        {
            return _semi_discrete_model.template transform_vector_to_field<linalg_backend_type>(
                _solution);
        }


      private:
        // the discrete system
        const semi_discrete_model_type & _semi_discrete_model;
        // the solver
        solver_type _solver;
        // the time step size
        scalar_type _dt;
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
        // the temporary vector, representing M * u_n, needed to compute the effective load vector
        vector_type _temp_vector;
        // the previous solution vector
        vector_type _solution;
    };


}    // namespace mito::timesteppers


// end of file
