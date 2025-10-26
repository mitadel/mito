// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // Class {FunctionSpace} represents a collection of finite elements of order {p} defined on a
    // manifold and subjected to a set of constraints.
    // TOFIX: add concept for element type
    template <class elementT, constraints::constraint_c constraintsT>
    class FunctionSpace {

      public:
        // the constraints type
        using constraints_type = constraintsT;
        // my template parameter, the finite element type
        using element_type = elementT;
        // typedef for a collection of finite elements
        using elements_type = utilities::segmented_vector_t<element_type>;
        // the degree of the finite element
        static constexpr int degree = element_type::degree;
        // the dimension of the physical space
        static constexpr int dim = element_type::dim;
        // assemble the mesh node type
        using mesh_node_type = geometry::node_t<dim>;
        // the discretization node type
        using discretization_node_type = typename element_type::discretization_node_type;
        // the constrained nodes type
        using constrained_nodes_type = std::set<discretization_node_type>;
        // the type of a map between the mesh nodes and discretization nodes
        using map_type = std::unordered_map<
            mesh_node_type, discretization_node_type, utilities::hash_function<mesh_node_type>>;


      public:
        // the constructor
        template <manifolds::manifold_c manifoldT>
        // require compatibility between the manifold cell and the finite element cell
        requires(std::is_same_v<
                    typename manifoldT::mesh_type::cell_type, typename element_type::cell_type>)
        constexpr FunctionSpace(const manifoldT & manifold, const constraints_type & constraints) :
            _elements(100),
            _constraints(constraints),
            _node_map()
        {
            // first order discretization
            if constexpr (degree == 1) {
                // discretize the manifold with first order continuous Galerkin
                discretize_triangleP1_CG(
                    manifold, constraints, _elements, _node_map, _constrained_nodes);
                // all done
                return;
            }

            // second order discretization
            if constexpr (degree == 2) {
                // discretize the manifold with second order continuous Galerkin
                discretize_triangleP2_CG(
                    manifold, constraints, _elements, _node_map, _constrained_nodes);
                // all done
                return;
            }
        }

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
