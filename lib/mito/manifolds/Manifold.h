// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    template <class cellT /* the type of cell */, int D /* spatial dimension */, class F>
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
        // the dimension of the manifold (that is the order of the cell)
        static constexpr int N = topology::order<cell_type>();
        // the dimension of the parametric space
        static constexpr int parametricDim = parametric_dim<cell_type>();
        // typedef for a point in parametric coordinates
        using parametric_point_type = manifolds::parametric_point_t<parametricDim>;
        // typedef for the metric field
        using metric_field_type = field_t<F>;

      public:
        inline Manifold(const mesh_type & mesh, const metric_field_type & metric) :
            _mesh(mesh),
            _metric(metric)
        {}

        inline ~Manifold() {}

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
        inline auto sanityCheck() -> bool
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

        inline auto elements() const noexcept -> const element_vector_t<cell_type> &
        {
            return _mesh.cells();
        }

        inline auto nElements() const noexcept -> int { return std::size(_mesh.cells()); }

        inline auto coordinates(const vertex_type & v) const -> const vector_t<D> &
        {
            // get the coordinates of the point attached to vertex {v}
            return _point(v)->coordinates();
        }

        inline auto parametrization(
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

        inline auto print() const -> void
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

      private:
        inline auto _point(const vertex_type & v) const -> const geometry::point_t<D> &
        {
            // look up the point attached to vertex {v}
            return _mesh.geometry().point(v);
        }

      private:
        // the underlying mesh
        const mesh_type & _mesh;
        // the metric field
        metric_field_type _metric;
    };

    template <class cellT, int D, class F>
    std::ostream & operator<<(std::ostream & os, const manifold_t<cellT, D, F> & manifold)
    {
        // print the manifold
        manifold.print();

        // all done
        return os;
    }

}    // namespace mito


#endif

// end of file
