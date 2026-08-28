// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // a contribution to a discrete system: a weakform to be assembled on a function space
    template <function_space_c functionSpaceT, class weakformT>
    struct Contribution {
        // my template parameters
        using function_space_type = functionSpaceT;
        using weakform_type = weakformT;

        // the function space
        const function_space_type & space;
        // the weakform
        const weakform_type & weakform;
    };

    template <class linearSystemT, contribution_c... contributionTs>
    class DiscreteSystem {

      private:
        // the linear system type
        using linear_system_type = linearSystemT;
        // the label type
        using label_type = std::string;
        // the function space type of the first contribution
        using first_function_space_type = typename std::tuple_element<
            0, std::tuple<contributionTs...>>::type::function_space_type;

      public:
        // the type of node
        using node_type = typename first_function_space_type::discretization_node_type;
        // require that all function spaces share the same discretization node type
        static_assert(
            (std::is_same_v<
                 node_type, typename contributionTs::function_space_type::discretization_node_type>
             && ...),
            "all function spaces must share the same discretization node type");

      private:
        // QUESTION: is std::map the best choice for {equation_map_type}?
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;
        // the constrained values type (map from constrained node to prescribed value)
        using constrained_values_type = typename first_function_space_type::constrained_values_type;
        // require that all function spaces prescribe values of the same type
        static_assert(
            (std::is_same_v<
                 constrained_values_type,
                 typename contributionTs::function_space_type::constrained_values_type>
             && ...),
            "all function spaces must prescribe constrained values of the same type");
        // TOFIX: what if the solution is not a scalar field? Generalize to different types of
        // solutions
        // the solution field type
        using solution_field_type = tensor::scalar_t;
        // the fem field type
        using fem_field_type = fem_field_t<solution_field_type>;

      public:
        // constructor
        constexpr DiscreteSystem(
            const label_type & label, const contributionTs &... contributions) :
            _contributions(contributions...),
            _equation_map(),
            _constrained_values(),
            _solution_field(_assemble_solution_field(label, contributions...)),
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
        // build a solution field on the union of the discretization nodes of all function spaces
        static auto _assemble_solution_field(
            const label_type & label, const contributionTs &... contributions) -> fem_field_type
        {
            // collect the discretization nodes of all the function spaces
            std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
            (get_discretization_nodes(contributions.space, nodes), ...);

            // build a nodal field on the collected discretization nodes
            return fem_field_type(
                discrete::nodal_field_t<solution_field_type>(nodes, label + ".solution"));
        }

        // build the equation map and return the number of equations
        auto _build_equation_map() -> int
        {
            // make a channel
            journal::info_t channel("discretization.discrete_system");

            // collect the nodes of all the function spaces
            std::set<node_type> nodes;
            std::apply(
                [&](const auto &... contribution) {
                    (get_discretization_nodes(contribution.space, nodes), ...);
                },
                _contributions);
            channel << "Number of nodes: " << std::size(nodes) << journal::endl;

            // merge the constrained nodes and their prescribed values of all the function spaces
            std::apply(
                [&](const auto &... contribution) {
                    (
                        [&] {
                            for (const auto & [node, value] :
                                 contribution.space.constrained_values()) {
                                auto [it, inserted] = _constrained_values.insert({ node, value });
                                // a node shared by multiple contributions must agree on its
                                // prescribed value
                                assert(inserted || it->second == value);
                            }
                        }(),
                        ...);
                },
                _contributions);
            channel << "Number of constrained nodes: " << std::size(_constrained_values)
                    << journal::endl;
            channel
                << "Number of interior nodes: " << std::size(nodes) - std::size(_constrained_values)
                << journal::endl;

            // populate the equation map (from node to equation, one equation per node)
            int equation = 0;

            // loop on all the nodes
            for (const auto & node : nodes) {
                if (_constrained_values.contains(node)) {
                    // mark the constrained node with a -1
                    _equation_map[node] = -1;
                } else {
                    // add the node to the equation map and increment the equation number
                    _equation_map[node] = equation++;
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

            // assemble one contribution
            auto assemble_one = [&](const auto & contribution) {
                // the function space type of this contribution
                using function_space_type =
                    typename std::remove_cvref_t<decltype(contribution)>::function_space_type;
                // the number of nodes per element
                constexpr int n_element_nodes = function_space_type::finite_element_type::n_nodes;

                // QUESTION: can we flip the element and block loops? What is the expected layout
                // in memory?
                //
                // loop on all the cells of the mesh
                for (const auto & element : contribution.space.elements()) {
                    // get the elementary contributions to matrix and right-hand side from the
                    // weakform
                    auto [elementary_matrix, elementary_vector] =
                        contribution.weakform.compute_blocks(element);

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
                                } else {
                                    // {node_b} is constrained: subtract the lift contribution of
                                    // its prescribed value from the right-hand side
                                    _linear_system.add_rhs_value(
                                        eq_a, -elementary_matrix[{ a, b }]
                                                  * _constrained_values.at(node_b));
                                }
                            });
                        }
                    });
                }
            };

            // assemble all the contributions
            std::apply(
                [&](const auto &... contribution) { (assemble_one(contribution), ...); },
                _contributions);
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
                    // populate the constrained node with its prescribed value
                    _solution_field(node) = _constrained_values.at(node);
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
        // the contributions to the discrete system
        std::tuple<contributionTs...> _contributions;

        // the equation map
        equation_map_type _equation_map;

        // the constrained nodes and their prescribed values, merged from all function spaces
        constrained_values_type _constrained_values;

        // the solution finite element field
        fem_field_type _solution_field;

        // the linear system of equations
        linear_system_type _linear_system;

        // the number of equations in the linear system
        int _n_equations = 0;
    };

}    // namespace mito


// end of file
