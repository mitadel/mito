// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    template <class functionSpaceT>
    class DiscreteSystem {

      private:
        // the function space type
        using function_space_type = functionSpaceT;
        // the type of node
        using node_type = typename function_space_type::discretization_node_type;
        // QUESTION: is std::map the best choice for {equation_map_type}?
        // the equation map type (map associating an equation number to each node degree of freedom)
        using equation_map_type = std::map<node_type, int>;

      public:
        // the default constructor
        constexpr DiscreteSystem(const function_space_type & function_space) :
            _function_space(function_space),
            _equation_map(),
            _n_equations(0)
        {
            _build_equation_map();
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
        // build the equation map
        void _build_equation_map()
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

            // the number of equations
            _n_equations = equation;

            // print the number of equations
            channel << "Number of equations: " << _n_equations << journal::endl;
        }

      public:
        // accessor to the equation map
        constexpr auto equation_map() const noexcept -> const equation_map_type &
        {
            return _equation_map;
        }

        // accessor to the number of equations
        constexpr auto n_equations() const noexcept -> int { return _n_equations; }

      private:
        // QUESTION: do we need the function space to be an attribute or do we only need it in the
        // constructor? Also, for sure we should have an attribute that is aware of the 'bricks' to
        // be assembled in the system. Either these bricks should be aware of the function space, or
        // the function space should know how to discretize the bricks with its own shape functions
        // a const reference to the function space
        const function_space_type & _function_space;

        // the equation map
        equation_map_type _equation_map;

        // the number of equations
        int _n_equations;
    };

}    // namespace mito


// end of file
