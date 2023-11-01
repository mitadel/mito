// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    template <metric_t metricT, class cellT /* the type of cell */, int D /* spatial dimension */>
    class Manifold {

      public:
        // the dimension of the physical space
        static constexpr int dim = D;

      private:
        // typedef for cell type
        using cell_type = cellT;
        // typedef for mesh type
        using mesh_type = mesh::mesh_t<cell_type, D>;
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
        // TOFIX: does it make sense to use the metric here since it cancels out with the inverse
        //  metric?
        template <int I>
        static constexpr auto _dx = one_form(_e<I>, identity_tensor_field<N, D>);
        // the metric volume form
        // TOFIX: make it general for N-dimension
        static constexpr auto _volume_form = sqrt(determinant(_metric)) * wedge(_dx<0>, _dx<1>);

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
        constexpr auto sanityCheck() -> bool
        {
            bool check = true;
            for (const auto & e : _mesh.cells()) {
                if (!e->sanityCheck()) {
                    std::cout << "Failed sanity check for element " << e << std::endl;
                    check = false;
                }
            }
            return check;
        }

        constexpr auto elements() const noexcept -> const element_vector_t<cell_type> &
        {
            return _mesh.cells();
        }

        constexpr auto nElements() const noexcept -> int { return std::size(_mesh.cells()); }

        constexpr auto coordinates(const vertex_type & v) const -> const vector_t<D> &
        {
            // get the coordinates of the point attached to vertex {v}
            return _point(v)->coordinates();
        }

        constexpr auto parametrization(
            const cell_type & cell, const parametric_point_type & point) const -> vector_t<D>
        {
            // collect the element vertices
            auto vertices = cell->vertices(vertices);
            vector_t<D> coordinates;
            // loop on vertices
            int v = 0;
            for (const auto & vertex : vertices) {
                const auto & vertexCoordinates = coordinates(vertex);
                coordinates += point[v] * vertexCoordinates;

                ++v;
            }

            return coordinates;
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
        constexpr auto e()
        {
            return _e<I>;
        }

        // get the I-th basis element for one-form fields
        template <int I>
        constexpr auto dx()
        {
            return _dx<I>;
        }

        // QUESTION: we can precompute the element volumes and store them in a data structure. Shall
        //  we do this here or at a higher level?
        // QUESTION: does this work for general metric tensors?
        constexpr auto volume() -> scalar_t
        {
            scalar_t volume = 0.0;

            for (const auto & cell : _mesh.cells()) {
                volume += _volume(cell);
            }

            // all done
            return volume;
        }

      private:
        constexpr auto _point(const vertex_type & v) const -> const geometry::point_t<D> &
        {
            // look up the point attached to vertex {v}
            return _mesh.geometry().point(v);
        }

        // helper function to pass from a collection of segments to a collection of
        // vectors associated to those segements
        template <int... J>
        constexpr auto _compute_director_vectors(
            const topology::edge_simplex_directors_t<N> & segments, integer_sequence<J...>) const
            -> auto
        {
            return std::array { _mesh.geometry().vector(segments[J])... };
        }

        // computes the volume of a cell
        constexpr auto _volume(const cell_type & cell) -> scalar_t
        {
            // get the director edges of this cell
            auto edge_directors = cell->directors();
            // compute the director vectors associated with each director edge
            auto directors = _compute_director_vectors(edge_directors, make_integer_sequence<N> {});
            // compute the volume of a N-order simplicial cell as (1/N) times the volume form
            // contracted with the cell directors
            // QUESTION: at what point should the volume be evaluated?
            // TOFIX: make it general for N-dimensions
            auto volume = cell->orientation() * 1.0 / N
                        * _volume_form({ 0.0, 0.0 })(directors[0], directors[1]);
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
