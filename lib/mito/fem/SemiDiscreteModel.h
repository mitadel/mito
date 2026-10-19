// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // TOFIX: for now the discrete system is one per function space. We should figure out a way to
    // extend the design to the case that there are multiple finite element discretizations that
    // end up on the same linear system.

    template <function_space_c functionSpaceT, class physicalModelT>
    class SemiDiscreteModel {

      public:
        // the archetype type of the physical model
        using archetype_type = typename physicalModelT::archetype_type;

      private:
        // the function space type
        using function_space_type = functionSpaceT;
        // the weakform type
        using weakform_type = decltype(semi_discrete_weakform(std::declval<physicalModelT>()));
        // the type of node
        using node_type = typename function_space_type::discretization_node_type;
        // QUESTION: is std::map the best choice for {equation_map_type}?
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;
        // the element type
        using finite_element_type = typename function_space_type::finite_element_type;
        // the number of nodes per element
        static constexpr int n_element_nodes = finite_element_type::n_nodes;

      public:
        // constructor
        constexpr SemiDiscreteModel(
            const function_space_type & function_space, const physicalModelT & physical_model) :
            _function_space(function_space),
            _physical_model(physical_model),
            _weakform(semi_discrete_weakform(physical_model)),
            _equation_map(),
            _n_equations(_build_equation_map())
        {
            // make a channel
            journal::info_t channel("discretization.discrete_system");

            // print the number of equations
            channel << "Number of equations: " << _n_equations << journal::endl;

            // all done
            return;
        }

        // destructor
        constexpr ~SemiDiscreteModel() = default;

        // delete move constructor
        constexpr SemiDiscreteModel(SemiDiscreteModel &&) noexcept = delete;

        // delete copy constructor
        constexpr SemiDiscreteModel(const SemiDiscreteModel &) = delete;

        // delete assignment operator
        constexpr SemiDiscreteModel & operator=(const SemiDiscreteModel &) = delete;

        // delete move assignment operator
        constexpr SemiDiscreteModel & operator=(SemiDiscreteModel &&) noexcept = delete;

        // assemble the matrix associated to a tag of the archetype
        template <class tagT, class matrixT>
        auto assemble_matrix(matrixT & _matrix) const -> void
        {
            for (const auto & element : _function_space.elements()) {
                auto elementary_matrix = _weakform.template compute_block<tagT>(element);
                emplace_elementary_matrix(elementary_matrix, _matrix, element);
            }
        }

        // assemble the vector associated to a tag of the archetype
        template <class tagT, class vectorT>
        auto assemble_vector(vectorT & _vector) const -> void
        {
            for (const auto & element : _function_space.elements()) {
                auto elementary_vector = _weakform.template compute_block<tagT>(element);
                emplace_elementary_vector(elementary_vector, _vector, element);
            }
        }

      private:
        // build the equation map and return the number of equations
        auto _build_equation_map() -> int
        {
            // make a channel
            journal::info_t channel("discretization.discrete_system");

            // get all the nodes in the function space
            std::set<node_type> nodes;
            get_discretization_nodes(_function_space, nodes);
            channel << "Number of nodes: " << std::size(nodes) << journal::endl;

            // get the constrained nodes in the function space
            const auto & constrained_nodes = _function_space.constrained_nodes();
            channel << "Number of constrained nodes: " << std::size(constrained_nodes)
                    << journal::endl;

            // get all the interior nodes as the difference between all the nodes and the boundary
            // nodes
            std::set<node_type> interior_nodes;
            std::set_difference(
                nodes.begin(), nodes.end(), constrained_nodes.begin(), constrained_nodes.end(),
                std::inserter(interior_nodes, interior_nodes.begin()));
            channel << "Number of interior nodes: " << std::size(interior_nodes) << journal::endl;

            // populate the equation map (from node to equation, one equations per node)
            int equation = 0;

            // loop on all the boundary nodes of the mesh
            for (const auto & node : constrained_nodes) {
                // check if the node is already in the equation map
                if (_equation_map.find(node) == _equation_map.end()) {
                    // add the node to the equation map with a -1 indicating that the node is on the
                    // boundary
                    _equation_map[node] = -1;
                }
            }

            // loop on all the interior nodes of the mesh
            for (const auto & node : interior_nodes) {
                // check if the node is already in the equation map
                if (_equation_map.find(node) == _equation_map.end()) {
                    // add the node to the equation map
                    _equation_map[node] = equation;
                    // increment the equation number
                    equation++;
                }
            }

            // return the number of equations
            return equation;
        }

        // emplace the elementary matrix into the global matrix
        template <class elementaryMatrixT, class matrixT>
        auto emplace_elementary_matrix(
            const elementaryMatrixT & elementary_matrix, matrixT & _matrix,
            const finite_element_type & element) const -> void
        {
            // loop on the a-th discretization node of the element
            tensor::constexpr_for_1<n_element_nodes>([&]<int a>() {
                // get the a-th discretization node of the element
                const auto & node_a = element.connectivity()[a];
                // get the equation number of {node_a}
                int eq_a = _equation_map.at(node_a);
                assert(eq_a < _n_equations);
                // non boundary nodes
                if (eq_a != -1) {
                    // loop on the b-th discretization node of the element
                    tensor::constexpr_for_1<n_element_nodes>([&]<int b>() {
                        // get the b-th discretization node of the element
                        const auto & node_b = element.connectivity()[b];
                        // get the equation number of {node_b}
                        int eq_b = _equation_map.at(node_b);
                        assert(eq_b < _n_equations);
                        // non boundary nodes
                        if (eq_b != -1) {
                            // assemble the value in the stiffness matrix
                            _matrix.add_matrix_value(eq_a, eq_b, elementary_matrix[{ a, b }]);
                        }
                    });
                }
            });
        }

        // emplace the elementary vector into the global vector
        template <class elementaryVectorT, class vectorT>
        auto emplace_elementary_vector(
            const elementaryVectorT & elementary_vector, vectorT & _vector,
            const finite_element_type & element) const -> void
        {
            // loop on the a-th discretization node of the element
            tensor::constexpr_for_1<n_element_nodes>([&]<int a>() {
                // get the a-th discretization node of the element
                const auto & node_a = element.connectivity()[a];
                // get the equation number of {node_a}
                int eq_a = _equation_map.at(node_a);
                assert(eq_a < _n_equations);
                // non boundary nodes
                if (eq_a != -1) {
                    // assemble the value in the right hand side
                    _vector.add_rhs_value(eq_a, elementary_vector[{ a }]);
                }
            });
        }

      public:
        // accessor to the number of equations
        constexpr auto n_equations() const noexcept -> int { return _n_equations; }

      private:
        // a const reference to the function space
        const function_space_type & _function_space;

        // a const reference to the physical model
        const physicalModelT & _physical_model;

        // the weakform
        weakform_type _weakform;

        // the equation map
        equation_map_type _equation_map;

        // the number of equations in the function space
        int _n_equations;
    };

}    // namespace mito


// end of file
