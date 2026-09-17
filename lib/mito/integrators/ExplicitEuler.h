// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::integrators {

    template <
        class discreteSystemT, class matrixSolverT,
        mito::math_backend::valid_backend_c mathBackendT>
    // TODO: require that the {matrixSolverT} is compatible with the linear system in
    // {discreteSystemT}
    class ExplicitEuler {

      private:
        // the discrete system type
        using discrete_system_type = discreteSystemT;
        // the matrix solver type
        using matrix_solver_type = matrixSolverT;
        // the math backend type
        using math_backend_type = mathBackendT;
        // the matrix type
        using matrix_type = typename math_backend_type::matrix_type;
        // the vector type
        using vector_type = typename math_backend_type::vector_type;
        // the index type
        using index_type = typename math_backend_type::index_type;
        // the scalar type
        using scalar_type = typename math_backend_type::scalar_type;
        // the options type
        using options_type = std::string;

      public:
        // the default constructor
        ExplicitEuler(discrete_system_type & discrete_system) :
            _discrete_system(discrete_system),
            _lumped_mass_vector("lumped_mass_vector", _discrete_system.n_equations()),
            _stiffness_matrix("stiffness_matrix", _discrete_system.n_equations()),
            _load_vector("load_vector", _discrete_system.n_equations()),
            _state_vector("state_vector", _discrete_system.n_equations()),
        {
            _discrete_system.template assemble_lumped_mass<mathBackendT>(_lumped_mass_vector);
            _discrete_system.template assemble_stiffness<mathBackendT>(_stiffness_matrix);
            _discrete_system.template assemble_load<mathBackendT>(_load_vector);
        }

        // initialise the solver
        auto initialiseSolver() -> void { ; }

        // initialise the time step
        auto initialiseTimeStep(scalar_type time, scalar_type dt) -> void { return; }

        auto integrateTimeStep(scalar_type time, scalar_type dt) -> void
        {
            // assemble the stiffness matrix and load vector
            VecPointwiseDivide();

            // all done
            return;
        }


        // print the matrix system
        auto print() const -> void { return _matrix_solver.print(); }

      private:
        // the discrete system
        discrete_system_type & _discrete_system;
        // the lumped mass vector
        vector_type _lumped_mass_vector;
        // the stiffness matrix
        matrix_type _stiffness_matrix;
        // the load vector
        vector_type _load_vector;
        // the state vector
        vector_type _state_vector;
    };


}    // namespace mito::fem


// end of file
