// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    template <metric_t metricT, class cellT /* the type of cell */, int D /* spatial dimension */>
    class Manifold {

      public:
        // the dimension of the physical space
        static constexpr int dim = D;
        // typedef for cell type
        using cell_type = cellT;
        // typedef for mesh type
        using mesh_type = mesh::mesh_t<cell_type, D>;
        // typedef for the cell type
        using cells_type = mesh_type::cells_type;

      private:
        // typedef for vertex
        using vertex_type = topology::vertex_t;
        // the metric type
        static constexpr metric_t metric_type = metricT;
        // the dimension of the manifold (that is the order of the cell)
        static constexpr int N = topology::order<cell_type>();
        // the dimension of the parametric space
        static constexpr int parametricDim = parametric_dim<cell_type>();
        // typedef for a point in parametric coordinates
        using parametric_point_type = manifolds::parametric_point_t<parametricDim>;

      private:
        // the metric field
        static constexpr auto _metric = metric<metric_type, N, D>::field();
        // basis for vector fields
        template <int I>
        static constexpr auto _e = uniform_field<D>(mito::e<I, N>);
        // basis for one-form fields
        // QUESTION: does it make sense to use the metric here since it cancels out with the inverse
        //  metric?
        template <int I>
        static constexpr auto _dx = one_form(_e<I>, identity_tensor_field<N, D>);
        // TOFIX: support 1D case
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
        constexpr Manifold(const mesh_type & mesh) : _mesh(mesh) {}

        constexpr ~Manifold() {}

      private:
        // delete default constructor
        Manifold() = delete;

        // delete copy constructor
        Manifold(const Manifold &) = delete;

        // delete move constructor
        Manifold(Manifold &&) = delete;

        // delete assignment operator
        Manifold & operator=(const Manifold &) = delete;

        // delete move assignment operator
        Manifold & operator=(Manifold &&) = delete;

      public:
        constexpr auto elements() const noexcept -> const cells_type & { return _mesh.cells(); }

        constexpr auto nElements() const noexcept -> int { return std::size(_mesh.cells()); }

        constexpr auto coordinates(const vertex_type & v) const -> const vector_t<D> &
        {
            // get the coordinates of the point attached to vertex {v}
            return _mesh.geometry().point(v)->coordinates();
        }

        constexpr auto parametrization(
            const cell_type & cell, const parametric_point_type & point) const -> vector_t<D>
        {
            vector_t<D> coord;
            // loop on the element vertices
            int v = 0;
            for (const auto & vertex : cell->vertices()) {
                const auto & vertexCoordinates = coordinates(vertex);
                coord += point[v] * vertexCoordinates;

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
                auto vertices = e->vertices();
                for (const auto & v : vertices) {
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

        // computes the volume of {cell} with the metric volume at {point}
        constexpr auto volume(
            const cell_type & cell,
            const geometry::coordinates_t<D> & point = mito::vector_t<D>()) const -> scalar_t
        {
            // all done
            return _volume(cell, point, make_integer_sequence<N> {});
        }

      private:
        // computes the volume of a cell
        template <int... J>
        constexpr auto _volume(
            const cell_type & cell, const geometry::coordinates_t<D> & point,
            integer_sequence<J...>) const -> scalar_t
        requires(sizeof...(J) == N)
        {
            // get the director edges of this cell
            auto directors = _mesh.geometry().directors(cell);
            // compute the volume of a N-order simplicial cell as (1/N!) times the volume form
            // contracted with the cell directors
            auto volume = 1.0 / pyre::tensor::factorial<N>() * _volume_form(point)(directors[J]...);
            // all done
            return volume;
        }

      private:
        // the underlying mesh
        const mesh_type & _mesh;
    };

    template <metric_t metricT, class cellT, int D>
    std::ostream & operator<<(std::ostream & os, const manifold_t<metricT, cellT, D> & manifold)
    {
        // print the manifold
        manifold.print();

        // all done
        return os;
    }

}    // namespace mito


#endif

// end of file
