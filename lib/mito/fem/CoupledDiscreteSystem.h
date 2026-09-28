// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // TOFIX: this duplicates the equation map and the scatter logic of {DiscreteSystem}; the two
    // should become one class once the coupled weakform has settled

    // Class {CoupledDiscreteSystem} assembles a {CoupledWeakform} into one linear system: the
    // degrees of freedom of all the function spaces among its terms are numbered together
    template <class linearSystemT, class coupledWeakformT>
    class CoupledDiscreteSystem {

      private:
        // the linear system type
        using linear_system_type = linearSystemT;
        // the coupled weakform type
        using coupled_weakform_type = coupledWeakformT;
        // the label type
        using label_type = std::string;
        // the terms of the weakform
        using terms_type = typename coupled_weakform_type::terms_type;
        // the source of the first term
        using first_source_type = typename std::tuple_element<0, terms_type>::type::source_type;

      public:
        // the type of node
        using node_type = typename first_source_type::discretization_node_type;
        // require that all the sources share the same discretization node type
        static_assert(
            []<class... termTs>(const std::tuple<termTs...> *) {
                return (
                    std::is_same_v<
                        node_type, typename termTs::source_type::discretization_node_type>
                    && ...);
            }(static_cast<terms_type *>(nullptr)),
            "all the term sources must share the same discretization node type");

      private:
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;
        // TOFIX: what if the solution is not a scalar field? Generalize to different types of
        // solutions
        // the solution field type
        using solution_field_type = tensor::scalar_t;
        // the constrained values type (map from constrained node to prescribed value)
        using constrained_values_type = std::map<node_type, solution_field_type>;
        // the fem field type
        using fem_field_type = fem_field_t<solution_field_type>;

      public:
        // constructor
        constexpr CoupledDiscreteSystem(
            const label_type & label, const coupled_weakform_type & weakform) :
            _weakform(weakform),
            _equation_map(),
            _constrained_values(),
            _solution_field(_assemble_solution_field(label, weakform)),
            _linear_system(label)
        {
            // make a channel
            journal::info_t channel("discretization.coupled_discrete_system");

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
        constexpr ~CoupledDiscreteSystem() = default;

        // delete move constructor
        constexpr CoupledDiscreteSystem(CoupledDiscreteSystem &&) noexcept = delete;

        // delete copy constructor
        constexpr CoupledDiscreteSystem(const CoupledDiscreteSystem &) = delete;

        // delete assignment operator
        constexpr CoupledDiscreteSystem & operator=(const CoupledDiscreteSystem &) = delete;

        // delete move assignment operator
        constexpr CoupledDiscreteSystem & operator=(CoupledDiscreteSystem &&) noexcept = delete;

      private:
        // collect the discretization nodes of the source of {term}, if it owns any
        template <class termT, class nodesT>
        static auto _collect_nodes(const termT & term, nodesT & nodes) -> void
        {
            if constexpr (function_space_c<typename termT::source_type>) {
                get_discretization_nodes(term.source, nodes);
            }
        }

        // build a solution field on the union of the discretization nodes of all function spaces
        static auto _assemble_solution_field(
            const label_type & label, const coupled_weakform_type & weakform) -> fem_field_type
        {
            // collect the discretization nodes of all the function spaces
            std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
            std::apply(
                [&](const auto &... term) { (_collect_nodes(term, nodes), ...); },
                weakform.terms());

            // build a nodal field on the collected discretization nodes
            return fem_field_type(
                discrete::nodal_field_t<solution_field_type>(nodes, label + ".solution"));
        }

        // merge the constrained values of the source of {term}, if it prescribes any
        template <class termT>
        auto _collect_constrained_values(const termT & term) -> void
        {
            if constexpr (function_space_c<typename termT::source_type>) {
                for (const auto & [node, value] : term.source.constrained_values()) {
                    [[maybe_unused]] auto [it, inserted] =
                        _constrained_values.insert({ node, value });
                    // a node constrained by more than one function space must agree on its
                    // prescribed value
                    assert(inserted || it->second == value);
                }
            }
        }

        // build the equation map and return the number of equations
        auto _build_equation_map() -> int
        {
            // make a channel
            journal::info_t channel("discretization.coupled_discrete_system");

            // collect the nodes of all the function spaces
            std::set<node_type> nodes;
            std::apply(
                [&](const auto &... term) { (_collect_nodes(term, nodes), ...); },
                _weakform.terms());
            channel << "Number of nodes: " << std::size(nodes) << journal::endl;

            // merge the constrained nodes and their prescribed values
            std::apply(
                [&](const auto &... term) { (_collect_constrained_values(term), ...); },
                _weakform.terms());
            channel << "Number of constrained nodes: " << std::size(_constrained_values)
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

        // scatter an elementary matrix into the linear system
        template <class connectivityT, class matrixT>
        auto _scatter_matrix(const connectivityT & connectivity, const matrixT & elementary_matrix)
            -> void
        {
            // the number of degrees of freedom of the element
            constexpr int N = std::tuple_size_v<std::remove_cvref_t<connectivityT>>;

            // loop on the a-th degree of freedom
            tensor::constexpr_for_1<N>([&]<int a>() {
                // get its equation number
                int eq_a = _equation_map.at(connectivity[a]);
                assert(eq_a < _n_equations);
                // constrained degrees of freedom carry no equation
                if (eq_a == -1) {
                    return;
                }
                // loop on the b-th degree of freedom
                tensor::constexpr_for_1<N>([&]<int b>() {
                    // get its equation number
                    int eq_b = _equation_map.at(connectivity[b]);
                    assert(eq_b < _n_equations);
                    if (eq_b != -1) {
                        // assemble the value in the stiffness matrix
                        _linear_system.add_matrix_value(eq_a, eq_b, elementary_matrix[{ a, b }]);
                    } else {
                        // the b-th degree of freedom is constrained: subtract the lift
                        // contribution of its prescribed value from the right-hand side
                        _linear_system.add_rhs_value(
                            eq_a,
                            -elementary_matrix[{ a, b }] * _constrained_values.at(connectivity[b]));
                    }
                });
            });
        }

        // scatter an elementary vector into the linear system
        template <class connectivityT, class vectorT>
        auto _scatter_vector(const connectivityT & connectivity, const vectorT & elementary_vector)
            -> void
        {
            // the number of degrees of freedom of the element
            constexpr int N = std::tuple_size_v<std::remove_cvref_t<connectivityT>>;

            // loop on the a-th degree of freedom
            tensor::constexpr_for_1<N>([&]<int a>() {
                // get its equation number
                int eq_a = _equation_map.at(connectivity[a]);
                assert(eq_a < _n_equations);
                // non constrained degrees of freedom
                if (eq_a != -1) {
                    // assemble the value in the right hand side
                    _linear_system.add_rhs_value(eq_a, elementary_vector[{ a }]);
                }
            });
        }

        // assemble one term of the weakform over its own elements
        template <class termT>
        auto _assemble_term(const termT & term) -> void
        {
            // the shape of the elementary contribution of this term
            using elementary_shape = typename termT::block_type::elementary_shape;

            // loop on the elements this term is assembled over
            for (const auto & element : term.source.elements()) {
                // matrix terms contribute to the left hand side, vector terms to the right
                if constexpr (tensor::matrix_c<elementary_shape>) {
                    _scatter_matrix(element.connectivity(), term.block.compute(element));
                } else {
                    _scatter_vector(element.connectivity(), term.block.compute(element));
                }
            }
        }

      public:
        // accessor to the linear system
        constexpr auto linear_system() noexcept -> linear_system_type & { return _linear_system; }

        // assemble the discrete system
        constexpr auto assemble() -> void
        {
            // check that the number of equations matches that of the linear system
            assert(_n_equations == _linear_system.n_equations());

            // assemble all the terms of the weakform
            std::apply(
                [&](const auto &... term) { (_assemble_term(term), ...); }, _weakform.terms());
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
        // the coupled weakform assembled by this system
        coupled_weakform_type _weakform;

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
