// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // Class {FunctionSpace} represents a collection of finite elements of order {p} defined on a
    // manifold and subjected to a set of constraints.
    // TOFIX: add concept for element type
    template <class finiteElementT, class manifoldT, constraints::constraint_c constraintsT>
    class FunctionSpace {

      public:
        // my template parameter, the finite element type
        using finite_element_type = finiteElementT;
        // the manifold type
        using manifold_type = manifoldT;
        // the constraints type
        using constraints_type = constraintsT;
        // the function space type
        using function_space_type =
            FunctionSpace<finite_element_type, manifold_type, constraints_type>;

        // my element view type
        using finite_elements_view_type = function_space_elements_view_t<function_space_type>;

        // the dimension of the physical space
        static constexpr int dim = finite_element_type::dim;
        // the mesh cell type
        using mesh_cell_type = typename finite_element_type::mesh_cell_type;
        // assemble the mesh node type
        using mesh_node_type = typename manifold_type::mesh_type::node_type;

        // the discretization node type
        using discretization_node_type = typename finite_element_type::discretization_node_type;
        // the constrained nodes type
        using constrained_nodes_type = std::set<discretization_node_type>;
        // the type of a map between the mesh nodes and discretization nodes
        using map_type = std::unordered_map<
            mesh_node_type, discretization_node_type, utilities::hash_function<mesh_node_type>>;

        // id type of a mesh cell
        using cell_id_type = utilities::index_t<mesh_cell_type>;
        // a collection of discretization discretization nodes
        using connectivity_type = typename finite_element_type::connectivity_type;
        // connectivity table type
        using connectivity_table_type = std::unordered_map<cell_id_type, connectivity_type>;

        // a finite element field type
        template <class fieldValueT>
        using fem_field_type = fem_field_t<fieldValueT>;

      public:
        // the constructor
        template <discretization_t discretizationT = discretization_t::CG>
        constexpr FunctionSpace(
            const manifold_type & manifold, const constraints_type & constraints) :
            _manifold(manifold),
            _constraints(constraints),
            _connectivity_table(),
            _node_map()
        {
            // TODO: merge the discretization type with the finite element type
            // discretize the manifold subject to the constraints
            discretize<finite_element_type, discretizationT>(
                manifold, constraints, _connectivity_table, _node_map, _constrained_nodes);
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
        // accessor to the underlying manifold
        constexpr auto manifold() const noexcept -> const manifold_type & { return _manifold; }

        // return an iterable view of the finite elements
        constexpr auto elements() const noexcept { return finite_elements_view_type{ *this }; }

      private:
        //  return the finite element associated to a mesh cell
        constexpr auto element(const mesh_cell_type & cell) const
        {
            // assemble and return the finite element from the manifold element and the localization
            // of the connectivity table to this cell
            // QUESTION: manifold element is morally the local geometry of the cell. Shall we call
            // it that way?
            return finite_element<finite_element_type>(
                _manifold.element(cell), _connectivity_table.at(cell.simplex().id()));
        }

      public:
        // TOFIX: not sure this should be constexpr
        // accessor for the constraints
        constexpr auto constraints() const noexcept -> const constraints_type &
        {
            return _constraints;
        }

        // get the constrained nodes
        constexpr auto constrained_nodes() const noexcept -> const constrained_nodes_type &
        {
            return _constrained_nodes;
        }

        // accessor for the node map
        constexpr auto node_map() const noexcept -> const map_type & { return _node_map; }

        // hand out an preallocated fem field for all the discretization nodes with name {name}
        template <class fieldValueT>
        constexpr auto fem_field(std::string name) const -> fem_field_type<fieldValueT>
        {
            // assemble the node type
            using node_type = discretization_node_type;

            // get the discretization nodes
            std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
            get_discretization_nodes(*this, nodes);

            // build a nodal field on the discretization nodes collected from the function space
            return fem_field_type<fieldValueT>(discrete::nodal_field_t<fieldValueT>(nodes, name));
        }

      private:
        // TOFIX: similarly to the manifold, which should not own (a reference to) the mesh, the
        // function space should not own (a reference to) the manifold.
        // the manifold should be able to endow the mesh elements with metric information, and the
        // function space should be able to endow the mesh elements with shape functions
        // Then we don't even need views!
        // to the manifold on which the function space is defined
        const manifold_type & _manifold;

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

        // the connectivity table of the finite elements
        connectivity_table_type _connectivity_table;

        // QUESTION: the reason why we need this map is to write the solution in the vtk writer file
        // (we need to know how the solution maps to the mesh nodes). I am not sure this is a good
        // reason to build and store this map, though. Also, if we plan to keep this map, we should
        // come up with a better name
        // TOFIX: discretization node should be a struct containing the mesh node and the local node
        // index, so that we can get rid of this map
        // a map between the mesh nodes and discretization nodes
        map_type _node_map;

        // frienship with the manifold elements view
        friend finite_elements_view_type;
    };

}    // namespace mito


// end of file
