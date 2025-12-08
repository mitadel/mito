// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // TOFIX: for now the discrete system is one per function space. We should figure out a way to
    // extend the design to the case that there are multiple finite element discretizations that
    // end up on the same linear system.

    template <function_space_c functionSpaceT, class linearSystemT>
    class DiscreteSystem {

      private:
        // the function space type
        using function_space_type = functionSpaceT;
        // the element type
        using element_type = typename function_space_type::element_type;
        // the weakform type
        using weakform_type = weakform_t<element_type>;
        // the linear system type
        using linear_system_type = linearSystemT;
        // the label type
        using label_type = std::string;
        // the type of node
        using node_type = typename function_space_type::discretization_node_type;
        // QUESTION: is std::map the best choice for {equation_map_type}?
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;
        // TOFIX: what if the solution is not a scalar field? Generalize to different types of
        // solutions
        // the solution field type
        using solution_field_type = tensor::scalar_t;
        // the fem field type
        using fem_field_type = fem_field_t<solution_field_type>;
        // the number of nodes per element
        static constexpr int n_element_nodes = element_type::n_nodes;

      public:
        // constructor
        constexpr DiscreteSystem(
            const label_type & label, const function_space_type & function_space,
            const weakform_type & weakform) :
            _function_space(function_space),
            _weakform(weakform),
            _equation_map(),
            _solution_field(
                function_space.template fem_field<solution_field_type>(label + ".solution")),
            _linear_system(label)
        {
            // make a channel
            journal::info_t channel("discretization.discrete_system");

            // build the equations map and get the number of equations
            _n_equations = _build_equation_map();

            // print the number of equations
            channel << "Number of equations: " << _n_equations << journal::endl;

            // create the linear system and allocate the memory
            _linear_system.create(_n_equations);

            // all done
            return;
        }

        // destructor
        constexpr ~DiscreteSystem() = default;

        // delete move constructor
        constexpr DiscreteSystem(DiscreteSystem &&) noexcept = delete;

        // delete copy constructor
        constexpr DiscreteSystem(const DiscreteSystem &) = delete;

        // delete assignment operator
        constexpr DiscreteSystem & operator=(const DiscreteSystem &) = delete;

        // delete move assignment operator
        constexpr DiscreteSystem & operator=(DiscreteSystem &&) noexcept = delete;

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

      public:
        // accessor to the linear system
        constexpr auto linear_system() noexcept -> linear_system_type & { return _linear_system; }

        // assemble the discrete system
        constexpr auto assemble() -> void
        {
            // check that the number of equations matches that of the linear system
            assert(_n_equations == _linear_system.n_equations());

            // QUESTION: can we flip the element and block loops? What is the expected layout in
            // memory?
            //
            // loop on all the cells of the mesh
            for (const auto & element : _function_space.elements()) {
                // get the elementary contributions to matrix and right-hand side from the weakform
                auto [elementary_matrix, elementary_vector] = _weakform.compute_blocks(element);

                // assemble the elementary blocks into the linear system of equations
                tensor::constexpr_for_1<n_element_nodes>([&]<int a>() {
                    // get the a-th discretization node of the element
                    const auto & node_a = element.connectivity()[a];
                    // get the equation number of {node_a}
                    int eq_a = _equation_map.at(node_a);
                    assert(eq_a < _n_equations);
                    // non boundary nodes
                    if (eq_a != -1) {
                        // assemble the value in the right hand side
                        _linear_system.add_rhs_value(eq_a, elementary_vector[{ a }]);
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
                                _linear_system.add_matrix_value(
                                    eq_a, eq_b, elementary_matrix[{ a, b }]);
                            }
                        });
                    }
                });
            }
        }

        // read the solution nodal field
        constexpr void read_solution()
        {
            // check that the number of equations matches that of the linear system
            assert(_n_equations == _linear_system.n_equations());

            // read the solution
            auto u = std::vector<double>(_n_equations);
            _linear_system.get_solution(u);

            // TODO: ask the function space to populate the constrained nodes appropriately

            // get the node map from the function space
            auto node_map = _function_space.node_map();
            // fill information in finite element field
            for (auto & [node, eq] : _equation_map) {
                if (eq != -1) {
                    // note the solution on the solution field
                    _solution_field(node) = u[eq];
                }
            }

            // all done
            return;
        }

        // accessor to the solution finite element field
        constexpr auto solution() const noexcept -> const fem_field_type &
        {
            return _solution_field;
        }

        // accessor to the number of equations
        constexpr auto n_equations() const noexcept -> int { return _n_equations; }

      private:
        // a const reference to the function space
        const function_space_type & _function_space;

        // the weakform
        const weakform_type & _weakform;

        // the equation map
        equation_map_type _equation_map;

        // the solution finite element field
        fem_field_type _solution_field;

        // the linear system of equations
        linear_system_type _linear_system;

        // the number of equations in the linear system
        int _n_equations = 0;
    };

}    // namespace mito


// end of file
