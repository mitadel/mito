// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // Class {FunctionSpace} represents a collection of finite elements of order {p} defined on a
    // manifold and subjected to a set of constraints.
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    class FunctionSpace {

      public:
        // the manifold type
        using manifold_type = manifoldT;
        // the constraints type
        using constraints_type = constraintsT;
        // the mesh type
        using mesh_type = typename manifold_type::mesh_type;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the mesh node type
        using mesh_node_type = typename cell_type::node_type;
        // the coordinate system type
        using coord_system_type = typename manifold_type::coordinate_system_type;
        // the degree of the finite element
        static constexpr int degree = p;
        // typedef for a finite element
        using element_type =
            typename isoparametric_simplex<degree, cell_type, coord_system_type>::type;
        // typedef for a collection of finite elements
        using elements_type = utilities::segmented_vector_t<element_type>;
        // the node type
        using discretization_node_type = typename element_type::node_type;
        // the nodes type
        using discretization_nodes_type = typename element_type::nodes_type;
        // the constrained nodes type
        using constrained_nodes_type = std::set<discretization_node_type>;

      private:
        // the type of a map between the mesh nodes and discretization nodes
        using map_type = std::unordered_map<
            mesh_node_type, discretization_node_type, utilities::hash_function<mesh_node_type>>;


      public:
        // the constructor
        constexpr FunctionSpace(
            const manifold_type & manifold, const constraints_type & constraints) :
            _elements(100),
            _constraints(constraints),
            _node_map()
        {
            // get the coordinate system of the manifold
            const auto & coord_system = manifold.coordinate_system();

            // loop on the cells of the mesh
            for (const auto & cell : manifold.elements()) {

                // get the nodes of the cell
                const auto & nodes = cell.nodes();

                // the origin of the coordinate system
                auto origin = typename coord_system_type::coordinates_type{};

                // the coordinates of the nodes of the triangle
                auto x_0 = coord_system.coordinates(nodes[0]->point()) - origin;
                auto x_1 = coord_system.coordinates(nodes[1]->point()) - origin;
                auto x_2 = coord_system.coordinates(nodes[2]->point()) - origin;

                // add the nodes to the map (if the mesh node is already present in the map, then
                // the present discretization node is used)
                auto node_0 =
                    _node_map.insert({ nodes[0], discretization_node_type() }).first->second;
                auto node_1 =
                    _node_map.insert({ nodes[1], discretization_node_type() }).first->second;
                auto node_2 =
                    _node_map.insert({ nodes[2], discretization_node_type() }).first->second;

                // create a finite element for each cell and add it to the pile
                _elements.emplace(
                    cell, discretization_nodes_type{ node_0, node_1, node_2 }, x_0, x_1, x_2);
            }

            // populate the constrained nodes
            for (const auto & cell : constraints.domain().cells()) {
                for (const auto & node : cell.nodes()) {
                    // get the discretization node associated with the mesh node from the map
                    auto it = _node_map.find(node);
                    // add the node to the constrained nodes
                    _constrained_nodes.insert(it->second);
                }
            }

            // all done
            return;
        };

        // destructor
        constexpr ~FunctionSpace() = default;

        // delete move constructor
        constexpr FunctionSpace(FunctionSpace &&) noexcept = delete;

        // delete copy constructor
        constexpr FunctionSpace(const FunctionSpace &) = delete;

        // delete assignment operator
        constexpr FunctionSpace & operator=(const FunctionSpace &) = delete;

        // delete move assignment operator
        constexpr FunctionSpace & operator=(FunctionSpace &&) noexcept = delete;

      public:
        // TOFIX: not sure this should be constexpr
        // accessor for the constraints
        constexpr auto constraints() const noexcept -> const constraints_type &
        {
            return _constraints;
        }

        // get the finite elements
        auto elements() const noexcept -> const elements_type & { return _elements; }

        // get the constrained nodes
        constexpr auto constrained_nodes() const noexcept -> const constrained_nodes_type &
        {
            return _constrained_nodes;
        }

        // accessor for the node map
        constexpr auto node_map() const noexcept -> const map_type & { return _node_map; }

      private:
        // a collection of finite elements
        elements_type _elements;

        // TOFIX: this should be a collection of constraints. Also, constraints may involve
        // different degrees of freedom (e.g. periodic boundary conditions to impose relations
        // between beam rotations). Therefore, the function space should be aware of the spatial
        // dimension of the shape functions.
        //
        // QUESTION: do we need to maintain a reference to the constraints?
        //
        // the constraints
        const constraints_type & _constraints;

        // the constrained nodes
        constrained_nodes_type _constrained_nodes;

        // QUESTION: the reason why we need this map is to write the solution in the vtk writer file
        // (we need to know how the solution maps to the mesh nodes). I am not sure this is a good
        // reason to build and store this map, though. Also, if we plan to keep this map, we should
        // come up with a better name
        // a map between the mesh nodes and discretization nodes
        map_type _node_map;
    };

}    // namespace mito


// end of file
