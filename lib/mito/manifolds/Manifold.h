// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    template <class cellT, geometry::coordinates_c coordsT, class metricVolumeFormT>
    requires(cellT::dim == coordsT::dim)
    class Manifold {

      private:
        // typedef for node
        using node_type = cellT::node_type;
        // the metric volume form type
        using metric_volume_form_type = metricVolumeFormT;
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = cellT::dim;
        // the dimension of the manifold (that is that of the cell)
        static constexpr int N = cellT::order;

      public:
        // my type
        using manifold_type = Manifold<cellT, coordsT, metric_volume_form_type>;
        // my element view type
        using manifold_elements_view_type = manifold_elements_view_t<manifold_type>;
        // typedef for cell type
        using cell_type = cellT;
        // typedef for a set of coordinates
        using coordinates_type = coordsT;
        // typedef for mesh type
        using mesh_type = mesh::mesh_t<cell_type>;
        // typedef for the cell type
        using cells_type = mesh_type::cells_type;
        // typedef for the atlas
        using atlas_type = atlas_t<cell_type, coordinates_type>;
        // typedef for a coordinates system
        using coordinate_system_type = atlas_type::coordinate_system_type;

      public:
        constexpr Manifold(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system,
            metric_volume_form_type metric_volume_form) :
            _mesh(mesh),
            _atlas(coordinate_system),
            _metric_volume_form(metric_volume_form)
        {}

        // destructor
        constexpr ~Manifold() = default;

        // default move constructor
        Manifold(Manifold &&) noexcept = default;

      private:
        // delete default constructor
        Manifold() = delete;

        // delete copy constructor
        Manifold(const Manifold &) = delete;

        // delete assignment operator
        Manifold & operator=(const Manifold &) = delete;

        // delete move assignment operator
        Manifold & operator=(Manifold &&) noexcept = delete;

      public:
        // accessor for the mesh
        constexpr auto mesh() const noexcept -> const mesh_type & { return _mesh; }

        // return an iterable view of the manifold elements
        constexpr auto elements() const noexcept { return manifold_elements_view_type{ *this }; }

      public:
        // return the manifold element associated to a cell
        constexpr auto element(const cell_type & cell) const
        {
            // get the parametrization of this cell
            auto phi = _atlas.parametrization(cell);
            // get the metric volume form of this cell
            auto w = _metric_volume_form;
            // assemble and return the manifold element
            return parametrized_element(cell, phi, w);
        }

      private:
        // the underlying mesh
        const mesh_type & _mesh;
        // the atlas
        atlas_type _atlas;
        // the metric volume form
        metric_volume_form_type _metric_volume_form;

        // frienship with the manifold elements view
        friend manifold_elements_view_type;
    };

}    // namespace mito


// end of file
