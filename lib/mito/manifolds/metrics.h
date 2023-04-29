// code guard
#if !defined(mito_manifold_metrics_h)
#define mito_manifold_metrics_h


namespace mito::manifolds {

    template <int D>
    auto computeSimplicesVolume(
        const element_vector_t<topology::simplex_t<D>> & elements,
        const mesh::vertex_point_table_t<D> & vertices, std::vector<real> & volumes) -> void
    {
        // number of element vertices
        constexpr int V = D + 1;

        // a container to store the coordinates of each vertex in a tensor
        matrix_t<V> pointsTensor;

        // assert memory allocation is consistent
        assert(volumes.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // reinitialize pointsTensor for a new element
            pointsTensor.reset();

            // use a set to collect vertices without repeated entries
            topology::vertex_set_t element_vertices;
            element->vertices(element_vertices);
            // assert you found V element vertices
            assert(V == element_vertices.size());

            // loop on element vertices
            int v = 0;
            for (const auto & vertex : element_vertices) {
                // fill up pointsTensor container
                for (int d = 0; d < D; ++d) {
                    pointsTensor[v * V + d] = vertices.find(vertex)->second->coordinates()[d];
                }
                pointsTensor[v * V + D] = 1.0;
                // update element vertices counter
                ++v;
            }

            // compute the volume of the e-th element
            volumes[e] =
                std::fabs(pyre::tensor::determinant(pointsTensor)) / pyre::tensor::factorial<D>();
            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <class element_t, int D>
    auto computeElementsVolume(
        const element_vector_t<element_t> & elements,
        const mesh::vertex_point_table_t<D> & vertices, std::vector<real> & volumes) -> void;

    template <>
    auto computeElementsVolume<topology::triangle_t, 2>(
        const element_vector_t<topology::triangle_t> & elements,
        const mesh::vertex_point_table_t<2> & vertices, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<2>(elements, vertices, volumes);
    }

    template <>
    auto computeElementsVolume<topology::tetrahedron_t, 3>(
        const element_vector_t<topology::tetrahedron_t> & elements,
        const mesh::vertex_point_table_t<3> & vertices, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<3>(elements, vertices, volumes);
    }

    template <>
    auto computeElementsVolume<topology::segment_t, 1>(
        const element_vector_t<topology::segment_t> & elements,
        const mesh::vertex_point_table_t<1> & vertices, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<1>(elements, vertices, volumes);
    }

    template <int D>
    auto computeSegmentsLength(
        const element_vector_t<topology::segment_t> & elements,
        const mesh::vertex_point_table_t<D> & vertices, std::vector<real> & length) -> void
    {
        // assert memory allocation is consistent
        assert(length.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            topology::vertex_set_t vertices_set;
            element->vertices(vertices_set);
            topology::vertex_vector_t element_vertices(vertices_set.begin(), vertices_set.end());

            // assert the size of vertices container is equal to the number of element vertices
            assert(element_vertices.size() == /*number of element vertices*/ 2);

            // store the geometry::distance between the two element vertices as the element length
            length[e] = geometry::distance(
                vertices.find(element_vertices[0])->second,
                vertices.find(element_vertices[1])->second);

            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <>
    auto computeElementsVolume<topology::segment_t, 2>(
        const element_vector_t<topology::segment_t> & elements,
        const mesh::vertex_point_table_t<2> & vertices, std::vector<real> & volumes) -> void
    {
        return computeSegmentsLength<2>(elements, vertices, volumes);
    }

    template <>
    auto computeElementsVolume<topology::segment_t, 3>(
        const element_vector_t<topology::segment_t> & elements,
        const mesh::vertex_point_table_t<3> & vertices, std::vector<real> & volumes) -> void
    {
        return computeSegmentsLength<3>(elements, vertices, volumes);
    }

    // follows implementation by Kahan2014
    template <int D = 3>
    auto computeTriangleArea(
        const element_vector_t<topology::triangle_t> & elements,
        const mesh::vertex_point_table_t<D> & vertices, std::vector<real> & areas) -> void
    {
        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            topology::vertex_set_t vertices_set;
            element->vertices(vertices_set);
            topology::vertex_vector_t element_vertices(vertices_set.begin(), vertices_set.end());

            // compute lengths of three edges
            std::array<real, 3> edges_lengths;
            edges_lengths[0] = geometry::distance(
                vertices.find(element_vertices[0])->second,
                vertices.find(element_vertices[1])->second);
            edges_lengths[1] = geometry::distance(
                vertices.find(element_vertices[0])->second,
                vertices.find(element_vertices[2])->second);
            edges_lengths[2] = geometry::distance(
                vertices.find(element_vertices[1])->second,
                vertices.find(element_vertices[2])->second);

            // sort edges lengths in ascending order
            std::sort(edges_lengths.begin(), edges_lengths.end());

            // a >= b >= c
            real a = edges_lengths[2];
            real b = edges_lengths[1];
            real c = edges_lengths[0];
            real amb = a - b;
            real bmc = b - c;
            real bpc = b + c;

            // compute area of element e
            areas[e] = 0.25 * sqrt((a + bpc) * (c - amb) * (c + amb) * (a + bmc));

            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <>
    auto computeElementsVolume<topology::triangle_t, 3>(
        const element_vector_t<topology::triangle_t> & elements,
        const mesh::vertex_point_table_t<3> & vertices, std::vector<real> & volumes) -> void
    {
        return computeTriangleArea(elements, vertices, volumes);
    }
}


#endif

// end of file
