// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <function_space_c spaceT>
    struct Contribution {
        const spaceT & space;
        const weakform_t<typename spaceT::element_type> & weakform;
        double coefficient;
    };

    template <class linearSystemT, function_space_c... functionSpaceTs>
    class DiscreteSystem {

      private:
        // the function space type
        using function_space_types = std::tuple<functionSpaceTs...>;
        // the linear system type
        using linear_system_type = linearSystemT;
        // the label type
        using label_type = std::string;

      public:
        // the type of node
        using node_type =
            typename std::tuple_element<0, function_space_types>::type::discretization_node_type;
        static_assert(
            (std::is_same_v<node_type, typename functionSpaceTs::discretization_node_type> && ...),
            "All function spaces must have the same discretization_node_type");

      private:
        // QUESTION: is std::map the best choice for {equation_map_type}?
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;
        // the prescribed values type (map associating a prescribed value to each constrained node)
        using prescribed_values_type = std::map<node_type, double>;
        // TOFIX: what if the solution is not a scalar field? Generalize to different types of
        // solutions
        // the solution field type
        using solution_field_type = tensor::scalar_t;
        // the fem field type
        using fem_field_type = discrete::nodal_field_t<solution_field_type>;

      public:
        // constructor
        constexpr DiscreteSystem(
            const label_type & label, Contribution<functionSpaceTs>... contributions) :
            _spaces(contributions.space...),
            _weakforms(contributions.weakform...),
            _coefficients{ contributions.coefficient... },
            _equation_map(),
            _solution_field(label + ".solution"),
            _linear_system(label)
        {
            // make a channel
            journal::info_t channel("discretization.discrete_system");

            // build the equations map and get the number of equations
            _n_equations = _build_equation_map();

            // register all nodes in the solution field
            for (const auto & [node, eq] : _equation_map) {
                _solution_field.insert(node);
            }

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

            std::set<node_type> nodes;
            std::set<node_type> constrained_nodes;

            // collect nodes from all function spaces
            std::apply(
                [&](const auto &... spaces) { (get_discretization_nodes(spaces, nodes), ...); },
                _spaces);
            channel << "Number of nodes: " << std::size(nodes) << journal::endl;

            // collect constrained nodes from all function spaces
            std::apply(
                [&](const auto &... spaces) {
                    (constrained_nodes.insert(
                         spaces.constrained_nodes().begin(), spaces.constrained_nodes().end()),
                     ...);
                },
                _spaces);
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

        // accessor to the global equation map (node -> equation index, or -1 if constrained)
        constexpr auto equation_map() const noexcept -> const equation_map_type &
        {
            return _equation_map;
        }

        // prescribed Dirichlet values
        constexpr auto prescribed_values() const noexcept -> const prescribed_values_type &
        {
            return _prescribed_values;
        }

        // set the prescribed value for a constrained (Dirichlet) node
        constexpr void set_prescribed_value(const node_type & node, double value)
        {
            _prescribed_values[node] = value;
        }

        // assemble the discrete system
        constexpr auto assemble() -> void
        {
            // check that the number of equations matches that of the linear system
            assert(_n_equations == _linear_system.n_equations());

            // assemble contributions from one (space, weakform, coefficient) triple
            auto assemble_one = [&](const auto & space, const auto & weakform, double coeff) {
                // n_nodes is deduced at compile time from the concrete space/element type
                constexpr int n_nodes = std::remove_cvref_t<decltype(space)>::element_type::n_nodes;

                // QUESTION: can we flip the element and block loops? What is the expected layout in
                // memory?
                //
                // loop on all the cells of the mesh
                for (const auto & element : space.elements()) {
                    // get the elementary contributions to matrix and right-hand side from the
                    // weakform
                    auto [elementary_matrix, elementary_vector] = weakform.compute_blocks(element);

                    // assemble the elementary blocks into the linear system of equations
                    tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                        // get the a-th discretization node of the element
                        const auto & node_a = element.connectivity()[a];
                        // get the equation number of {node_a}
                        int eq_a = _equation_map.at(node_a);
                        assert(eq_a < _n_equations);
                        // non boundary nodes
                        if (eq_a != -1) {
                            // assemble the value in the right hand side
                            _linear_system.add_rhs_value(eq_a, coeff * elementary_vector[{ a }]);
                            // loop on the b-th discretization node of the element
                            tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                                // get the b-th discretization node of the element
                                const auto & node_b = element.connectivity()[b];
                                // get the equation number of {node_b}
                                int eq_b = _equation_map.at(node_b);
                                assert(eq_b < _n_equations);
                                // non boundary nodes
                                if (eq_b != -1) {
                                    // assemble the value in the stiffness matrix
                                    _linear_system.add_matrix_value(
                                        eq_a, eq_b, coeff * elementary_matrix[{ a, b }]);
                                } else {
                                    // constrained node: subtract the lift contribution from the RHS
                                    auto it = _prescribed_values.find(node_b);
                                    if (it != _prescribed_values.end()) {
                                        _linear_system.add_rhs_value(
                                            eq_a,
                                            -coeff * elementary_matrix[{ a, b }] * it->second);
                                    }
                                }
                            });
                        }
                    });
                }
            };

            // invoke assemble_one for each space using index_sequence
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (assemble_one(std::get<Is>(_spaces), std::get<Is>(_weakforms), _coefficients[Is]),
                 ...);
            }(std::index_sequence_for<functionSpaceTs...>{});
        }

        // read the solution nodal field
        constexpr void read_solution()
        {
            // check that the number of equations matches that of the linear system
            assert(_n_equations == _linear_system.n_equations());

            // read the solution
            auto u = std::vector<double>(_n_equations);
            _linear_system.get_solution(u);

            // fill information in finite element field
            for (auto & [node, eq] : _equation_map) {
                if (eq != -1) {
                    // note the solution on the solution field
                    _solution_field(node) = u[eq];
                } else {
                    // populate constrained nodes with their prescribed values
                    auto it = _prescribed_values.find(node);
                    if (it != _prescribed_values.end()) {
                        _solution_field(node) = it->second;
                    }
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
        // the function spaces
        std::tuple<const functionSpaceTs &...> _spaces;

        // the weakforms associated to each space
        std::tuple<const weakform_t<typename functionSpaceTs::element_type> &...> _weakforms;

        // the coefficients for each space
        std::array<double, sizeof...(functionSpaceTs)> _coefficients;

        // the equation map
        equation_map_type _equation_map;

        // prescribed values for constrained (Dirichlet) nodes
        prescribed_values_type _prescribed_values;

        // the solution finite element field
        fem_field_type _solution_field;

        // the linear system of equations
        linear_system_type _linear_system;

        // the number of equations in the linear system
        int _n_equations = 0;
    };

}    // namespace mito


// end of file
