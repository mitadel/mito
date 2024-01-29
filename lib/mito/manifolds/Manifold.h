// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    template <class cellT, geometry::CoordinateType coordsT>
    class Manifold {

      private:
        // typedef for node
        using node_type = cellT::node_type;
        // the coordinates type
        static constexpr geometry::CoordinateType coords_type = coordsT;
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
        using coordinates_type = geometry::coordinates_t<D, coords_type>;
        // typedef for a coordinates system
        using coordinate_system_type = geometry::coordinate_system_t<D, coords_type>;

      private:
        // the metric field
        static constexpr auto _metric = metric<coords_type, N, D>::field();
        // basis for vector fields
        template <int I>
        static constexpr auto _e = uniform_field<D, coords_type>(mito::e<I, N>);
        // basis for one-form fields
        // QUESTION: does it make sense to use the metric here since it cancels out with the inverse
        //  metric?
        template <int I>
        static constexpr auto _dx = one_form(_e<I>, identity_tensor_field<N, D, coords_type>);
        // helper function wedging the N basis 1-forms
        template <int... J>
        static constexpr auto _wedge(integer_sequence<J...>)
        requires(sizeof...(J) == N)
        {
            // return the basis N-form
            return wedge(_dx<J>...);
        }
        // the metric volume form
        static constexpr auto _volume_form =
            sqrt(determinant(_metric)) * _wedge(make_integer_sequence<N> {});

      public:
        constexpr Manifold(
            const mesh_type & mesh, const coordinate_system_type & coordinate_system) :
            _mesh(mesh),
            _coordinate_system(coordinate_system)
        {}

        constexpr ~Manifold() {}

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

        // get the I-th basis element for vector fields
        template <int I>
        constexpr auto e() const
        {
            return _e<I>;
        }

        // get the I-th basis element for one-form fields
        template <int I>
        constexpr auto dx() const
        {
            return _dx<I>;
        }

        // QUESTION: we can precompute the element volumes and store them in a data structure. Shall
        //  we do this here or at a higher level?
        // QUESTION: does this work for general metric tensors?
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
            return _volume(cell, make_integer_sequence<N> {});
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
    };

    template <class cellT, geometry::CoordinateType coordsT>
    std::ostream & operator<<(std::ostream & os, const manifold_t<cellT, coordsT> & manifold)
    {
        // print the manifold
        manifold.print();

        // all done
        return os;
    }

}    // namespace mito


#endif

// end of file
