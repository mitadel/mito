// code guard
#if !defined(mito_geometry_metrics_h)
#define mito_geometry_metrics_h


namespace mito::geometry {

    template <int D>
    auto distance(const point_t<D> & pointA, const point_t<D> & pointB) -> real
    {
        // return the distance between the two points
        auto dist = pointA - pointB;
        return sqrt(dist * dist);
    }

    template <int D>
    auto computeSimplicesVolume(
        const element_vector_t<simplex_t<D>> & elements, std::vector<real> & volumes) -> void
    {
        // number of element vertices
        constexpr int V = D + 1;

        // a container to store the coordinates of each vertex in a tensor
        static matrix_t<V> pointsTensor;

        // assert memory allocation is consistent
        assert(volumes.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // reinitialize pointsTensor for a new element
            pointsTensor.reset();

            // use a set to collect vertices without repeated entries
            vertex_set_t element_vertices;
            element->vertices(element_vertices);
            // assert you found V element vertices
            assert(V == element_vertices.size());

            // loop on element vertices
            int v = 0;
            for (const auto & vertex : element_vertices) {
                // fill up pointsTensor container
                for (int d = 0; d < D; ++d) {
                    pointsTensor[v * V + d] = point_cloud<D>::point(vertex)[d];
                }
                pointsTensor[v * V + D] = 1.0;
                // update element vertices counter
                ++v;
            }

            // compute the volume of the e-th element
            volumes[e] =
                fabs(pyre::algebra::determinant(pointsTensor)) / pyre::algebra::factorial<D>();
            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <class element_t, int D>
    auto computeElementsVolume(
        const element_vector_t<element_t> & elements, std::vector<real> & volumes) -> void;

    template <>
    auto computeElementsVolume<triangle_t, 2>(
        const element_vector_t<triangle_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<2>(elements, volumes);
    }

    template <>
    auto computeElementsVolume<tetrahedron_t, 3>(
        const element_vector_t<tetrahedron_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<3>(elements, volumes);
    }

    template <>
    auto computeElementsVolume<segment_t, 1>(
        const element_vector_t<segment_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeSimplicesVolume<1>(elements, volumes);
    }

    template <int D>
    auto computeSegmentsLength(
        const element_vector_t<segment_t> & elements, std::vector<real> & length) -> void
    {
        // assert memory allocation is consistent
        assert(length.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            vertex_set_t vertices_set;
            element->vertices(vertices_set);
            vertex_vector_t element_vertices(vertices_set.begin(), vertices_set.end());

            // assert the size of vertices container is equal to the number of element vertices
            assert(element_vertices.size() == /*number of element vertices*/ 2);

            // store the distance between the two element vertices as the element length
            length[e] = distance(
                point_cloud<D>::point(element_vertices[0]),
                point_cloud<D>::point(element_vertices[1]));

            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <>
    auto computeElementsVolume<segment_t, 2>(
        const element_vector_t<segment_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeSegmentsLength<2>(elements, volumes);
    }

    template <>
    auto computeElementsVolume<segment_t, 3>(
        const element_vector_t<segment_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeSegmentsLength<3>(elements, volumes);
    }

    // follows implementation by Kahan2014
    template <int D = 3>
    auto computeTriangleArea(
        const element_vector_t<triangle_t> & elements, std::vector<real> & areas) -> void
    {
        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            vertex_set_t vertices_set;
            element->vertices(vertices_set);
            vertex_vector_t element_vertices(vertices_set.begin(), vertices_set.end());

            // compute lengths of three edges
            std::array<real, 3> edges_lengths;
            edges_lengths[0] = distance(
                point_cloud<D>::point(element_vertices[0]),
                point_cloud<D>::point(element_vertices[1]));
            edges_lengths[1] = distance(
                point_cloud<D>::point(element_vertices[0]),
                point_cloud<D>::point(element_vertices[2]));
            edges_lengths[2] = distance(
                point_cloud<D>::point(element_vertices[1]),
                point_cloud<D>::point(element_vertices[2]));

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
    auto computeElementsVolume<triangle_t, 3>(
        const element_vector_t<triangle_t> & elements, std::vector<real> & volumes) -> void
    {
        return computeTriangleArea(elements, volumes);
    }
}


#endif

// end of file
