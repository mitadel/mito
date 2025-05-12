// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
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
        // the node type
        using node_type = typename cell_type::node_type;
        // the coordinate system type
        using coord_system_type = typename manifold_type::coordinate_system_type;
        // TOFIX: order and degree are hard coded here for now. Eventually, we should make them
        // template parameters for the class
        // typedef for a finite element
        using element_type =
            typename isoparametric_simplex<2, 1, cell_type, coord_system_type>::type;
        // typedef for a collection of finite elements
        using elements_type = utilities::segmented_vector_t<element_type>;

      public:
        // the constructor
        constexpr FunctionSpace(
            const manifold_type & manifold, const constraints_type & constraints) :
            _elements(100),
            _constraints(constraints)
        {
            // get the coordinate system of the manifold
            const auto & coord_system = manifold.coordinate_system();

            // loop on the cells of the mesh
            for (const auto & cell : manifold.elements()) {
                // create a finite element for each cell and add it to the pile
                _elements.emplace(cell, coord_system);
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

      private:
        // a collection of finite elements
        elements_type _elements;

        // TOFIX: this should be a collection of constraints. Also, constraints may involve
        // different degrees of freedom (e.g. periodic boundary conditions to impose relations
        // between beam rotations). Therefore, the function space should be aware of the spatial
        // dimension of the shape functions.
        //
        // the constraints
        const constraints_type & _constraints;
    };

}    // namespace mito


// end of file
