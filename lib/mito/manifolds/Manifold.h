// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    template <class cellT, geometry::coordinates_c coordsT, class metricFieldT>
    requires(cellT::dim == coordsT::dim)
    class Manifold {

      private:
        // typedef for node
        using node_type = cellT::node_type;
        // the metric field type
        using metric_field_type = metricFieldT;
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = cellT::dim;
        // the dimension of the manifold (that is that of the cell)
        static constexpr int N = cellT::order;

      public:
        // typedef for cell type
        using cell_type = cellT;
        // typedef for mesh type
        using mesh_type = mesh::mesh_t<cell_type>;
        // typedef for the cell type
        using cells_type = mesh_type::cells_type;
        // typedef for a set of coordinates
        using coordinates_type = coordsT;
        // typedef for a coordinates system
        using coordinate_system_type = geometry::coordinate_system_t<coordinates_type>;

      public:
        constexpr Manifold(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system,
            metric_field_type metric_field) :
            _mesh(mesh),
            _coordinate_system(coordinate_system),
            _metric_field(metric_field)
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

        // accessor for the coordinate system
        constexpr auto coordinate_system() const noexcept -> const coordinate_system_type &
        {
            return _coordinate_system;
        }

        constexpr auto elements() const noexcept -> const cells_type & { return _mesh.cells(); }

        constexpr auto nElements() const noexcept -> int { return std::size(_mesh.cells()); }

        constexpr auto coordinates(const node_type & v) const -> const coordinates_type &
        {
            // get the coordinates of the point attached to vertex {v}
            return _coordinate_system.coordinates(v->point());
        }

        constexpr auto print() const -> void
        {
            // make a channel
            journal::info_t channel("mito.manifold");

            // print the element set of the manifold
            channel << "Element set: " << journal::endl;

            for (const auto & e : _mesh.cells()) {
                // print the elemental composition
                channel << "Composition: " << journal::endl;
                channel << e;
                // and the coordinates of the vertices
                channel << "Vertices: " << journal::endl;
                auto nodes = e.nodes();
                for (const auto & v : nodes) {
                    channel << coordinates(v) << journal::endl;
                }
                channel << journal::endl;
            }
        }

        constexpr auto volume() const -> tensor::scalar_t
        {
            tensor::scalar_t result = 0.0;
            for (const auto & cell : _mesh.cells()) {
                result += volume(cell);
            }
            // all done
            return result;
        }

        // computes the volume of {cell}
        constexpr auto volume(const cell_type & cell) const -> tensor::scalar_t
        {
            // all done
            return _volume(cell, tensor::make_integer_sequence<N>{});
        }

      private:
        // computes the volume of a cell via Riemannian formula (1/N!) * sqrt(det(J^T g J))
        template <int... Is>
        constexpr auto _volume(const cell_type & cell, tensor::integer_sequence<Is...>) const
            -> tensor::scalar_t
        requires(sizeof...(Is) == N)
        {
            // get the director edges of this cell and the point where they stem from
            auto [point, directors] = mito::geometry::directors(cell, _coordinate_system);
            // evaluate the ambient metric at the base point
            auto g = _metric_field(point);
            // assemble the Jacobian from the directors (D x N matrix)
            auto J = mito::tensor::matrix_from_columns<N, D>(directors);
            // compute the volume as (1/N!) * sqrt(det(J^T g J))
            return 1.0 / mito::tensor::factorial<N>()
                 * std::sqrt(mito::tensor::determinant(mito::tensor::transpose(J) * g * J));
        }

      private:
        // the underlying mesh
        const mesh_type & _mesh;
        // the coordinate system
        const coordinate_system_type & _coordinate_system;
        // the metric field
        metric_field_type _metric_field;
    };

}    // namespace mito


// end of file
