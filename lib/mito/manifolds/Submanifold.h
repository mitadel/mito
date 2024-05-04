// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_Submanifold_h)
#define mito_manifolds_Submanifold_h


namespace mito::manifolds {

    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    requires(cellT::dim == coordsT::dim)
    class Submanifold {

      private:
        // typedef for node
        using node_type = cellT::node_type;
        // the volume form type
        using volume_form_type = volumeFormT;
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = cellT::dim;
        // the dimension of the manifold (that is that of the cell)
        static constexpr int N = cellT::order;
        // the dimension of the parametric space
        static constexpr int parametricDim = parametric_dim<typename cellT::simplex_type>();
        // typedef for a point in parametric coordinates
        using parametric_point_type = manifolds::parametric_point_t<parametricDim>;

      public:
        // the dimension of the physical space
        static constexpr int dim = D;
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
        constexpr Submanifold(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system,
            volume_form_type volume_form) :
            _mesh(mesh),
            _coordinate_system(coordinate_system),
            _volume_form(volume_form)
        {}

        // destructor
        constexpr ~Submanifold() = default;

        // default move constructor
        Submanifold(Submanifold &&) noexcept = default;

      private:
        // delete default constructor
        Submanifold() = delete;

        // delete copy constructor
        Submanifold(const Submanifold &) = delete;

        // delete assignment operator
        Submanifold & operator=(const Submanifold &) = delete;

        // delete move assignment operator
        Submanifold & operator=(Submanifold &&) noexcept = delete;

      public:
        constexpr auto elements() const noexcept -> const cells_type & { return _mesh.cells(); }

        constexpr auto nElements() const noexcept -> int { return std::size(_mesh.cells()); }

        constexpr auto coordinates(const node_type & v) const -> const coordinates_type &
        {
            // get the coordinates of the point attached to vertex {v}
            return _coordinate_system.coordinates(v.point());
        }

        constexpr auto parametrization(
            const cell_type & cell, const parametric_point_type & point) const -> coordinates_type
        {
            coordinates_type coord;
            // loop on the element nodes
            int v = 0;
            for (const auto & node : cell.nodes()) {
                const auto & nodeCoordinates = coordinates(node);
                coord += point[v] * nodeCoordinates;

                ++v;
            }

            return coord;
        }

        constexpr auto print() const -> void
        {
            // print the element set of the manifold
            std::cout << "Element set: " << std::endl;

            for (const auto & e : _mesh.cells()) {
                // print the elemental composition
                std::cout << "Composition: " << std::endl;
                std::cout << e;
                // and the coordinates of the vertices
                std::cout << "Vertices: " << std::endl;
                auto nodes = e.nodes();
                for (const auto & v : nodes) {
                    std::cout << coordinates(v) << std::endl;
                }
                std::cout << std::endl;
            }
        }

        constexpr auto volume() const -> scalar_t
        {
            scalar_t result = 0.0;
            for (const auto & cell : _mesh.cells()) {
                result += volume(cell);
            }
            // all done
            return result;
        }

        // computes the volume of {cell}
        constexpr auto volume(const cell_type & cell) const -> scalar_t
        {
            // all done
            return _volume(cell, make_integer_sequence<N>{});
        }

      private:
        // computes the volume of a cell
        template <int... J>
        constexpr auto _volume(const cell_type & cell, integer_sequence<J...>) const -> scalar_t
        requires(sizeof...(J) == N)
        {
            // get the director edges of this cell and the point where they stem from
            auto [point, directors] = mito::geometry::directors(cell, _coordinate_system);
            // compute the volume of a N-order simplicial cell as (1/N!) times the volume form
            // contracted with the cell directors
            auto volume = 1.0 / pyre::tensor::factorial<N>() * _volume_form(point)(directors[J]...);
            // all done
            return volume;
        }

      private:
        // the underlying mesh
        const mesh_type & _mesh;
        // the coordinate system
        const coordinate_system_type & _coordinate_system;
        // the volume form
        volume_form_type _volume_form;
    };

}    // namespace mito


#endif

// end of file
