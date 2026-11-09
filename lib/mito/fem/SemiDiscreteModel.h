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
        // the solution field type
        using solution_field_type = tensor::scalar_t;
        // the fem field type
        using fem_field_type = fem_field_t<solution_field_type>;
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

        // set the matrix pattern
        template <linalg::valid_backend_c backendT>
        auto set_matrix_pattern(typename backendT::matrix_type & matrix) const -> void
        {
            // the index type
            using index_type = typename backendT::index_type;

            // the row and column indices of all element entries
            std::vector<index_type> row_indices, col_indices;

            // the number of elements
            std::size_t n_elements = 0;

            // loop on all the elements
            for (const auto & element : _function_space.elements()) {
                tensor::constexpr_for_1<n_element_nodes>([&]<int a>() {
                    const index_type eq_a = _equation_map.at(element.connectivity()[a]);
                    tensor::constexpr_for_1<n_element_nodes>([&]<int b>() {
                        const index_type eq_b = _equation_map.at(element.connectivity()[b]);
                        row_indices.push_back(eq_a);
                        col_indices.push_back(eq_b);
                    });
                });

                ++n_elements;
            }

            // one segment as every element has the same number of nodes
            matrix.set_pattern(
                std::move(row_indices), std::move(col_indices),
                { { static_cast<std::size_t>(n_element_nodes), n_elements } });
        }

        // assemble the matrix associated to a tag of the archetype
        template <class tagT, linalg::valid_backend_c backendT>
        auto set_matrix(typename backendT::matrix_type & matrix) const -> void
        {
            // the element values, in the same order as the pattern
            std::vector<typename backendT::scalar_type> values;

            // loop on all the elements
            for (const auto & element : _function_space.elements()) {
                auto elementary_matrix = _weakform.template compute_block<tagT>(element);
                tensor::constexpr_for_1<n_element_nodes>([&]<int a>() {
                    tensor::constexpr_for_1<n_element_nodes>(
                        [&]<int b>() { values.push_back(elementary_matrix[{ a, b }]); });
                });
            }

            // set the values in the matrix
            matrix.set_values(values);
        }

        // assemble the vector associated to a tag of the archetype
        template <class tagT, linalg::valid_backend_c backendT>
        auto assemble_vector(typename backendT::vector_type & vector) const -> void
        {
            for (const auto & element : _function_space.elements()) {
                auto elementary_vector = _weakform.template compute_block<tagT>(element);
                emplace_elementary_vector(elementary_vector, vector, element);
            }
        }

        // transform a data vector into a finite element field
        template <linalg::valid_backend_c linalgBackendT>
        constexpr auto transform_vector_to_field(
            const typename linalgBackendT::vector_type & solution_vector) const -> fem_field_type
        {
            // the solution finite element field
            fem_field_type solution_field =
                _function_space.template fem_field<solution_field_type>("solution_field");

            // check that the number of equations matches that of the linear system
            assert(_n_equations == solution_vector.size());

            // read the solution
            auto u = std::vector<double>(_n_equations);
            solution_vector.get_vector(u);

            // fill information in finite element field
            for (auto & [node, eq] : _equation_map) {
                if (eq != -1) {
                    // note the solution on the solution field
                    solution_field(node) = u[eq];
                }
            }

            // all done
            return solution_field;
        }

        // import continuous field
        template <linalg::valid_backend_c linalgBackendT>
        constexpr auto transform_field_to_vector(
            const fields::field_c auto & continuous_field) const -> linalgBackendT::vector_type
        {
            // the vertex field
            auto vertex_field = discrete::mesh_field(
                _function_space.manifold().mesh(),
                _function_space.manifold().atlas().coordinate_system(), continuous_field,
                "discrete_field");

            // the solution vector
            typename linalgBackendT::vector_type solution_vector("solution_vector", _n_equations);

            // fill information in finite element field
            for (auto & [mesh_node, node] : _function_space.node_map()) {
                if (int eq = _equation_map.at(node); eq != -1) {
                    // note the solution on the solution field
                    solution_vector.insert_value(eq, vertex_field(mesh_node));
                }
            }

            // all done
            return solution_vector;
        }

        // import discrete field
        template <linalg::valid_backend_c linalgBackendT>
        constexpr auto transform_field_to_vector(const fem_field_type & discrete_field) const
            -> linalgBackendT::vector_type
        {
            // the solution vector
            typename linalgBackendT::vector_type solution_vector("solution_vector", _n_equations);

            // fill information in finite element field
            for (auto & [node, eq] : _equation_map) {
                if (eq != -1) {
                    // note the solution on the solution field
                    solution_vector.insert_value(eq, discrete_field(node));
                }
            }

            // all done
            return solution_vector;
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

        // emplace the elementary vector into the global vector
        template <class elementaryVectorT, class vector_type, class finiteElementT>
        auto emplace_elementary_vector(
            const elementaryVectorT & elementary_vector, vector_type & _vector,
            const finiteElementT & element) const -> void
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
                    _vector.add_value(eq_a, elementary_vector[{ a }]);
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
