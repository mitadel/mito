// code guard
#if !defined(mito_mesh_VertexSet_h)
#define mito_mesh_VertexSet_h


namespace mito::mesh {

    template <int D>
    class VertexSet {

        // using map_t = std::unordered_map<std::reference_wrapper<vertex_t>, point_t<D>>;
        // TODO: test O for access
        using map_t = std::unordered_map<const vertex_t *, const point_t<D>>;

      public:
        VertexSet() : _map() {};

        ~VertexSet() {}

      private:
        // delete copy constructor
        VertexSet(const VertexSet &) = delete;

        // delete move constructor
        VertexSet(const VertexSet &&) = delete;

        // delete assignment operator
        const VertexSet & operator=(const VertexSet &) = delete;

        // delete move assignment operator
        const VertexSet & operator=(const VertexSet &&) = delete;

      public:
        void print() const
        {
            // iterate on map
            for (auto item : _map) {
                std::cout << "Vertex: " << item.first << std::endl;
                std::cout << "Point: " << item.second << std::endl;
            }
            // all done
            return;
        }

        int size() { return _map.size(); }

        auto insert(const vertex_t & vertex, const point_t<D> & point)
        {
            return _map.insert(
                // std::pair<std::reference_wrapper<vertex_t>, point_t<D>>(vertex, point)
                std::pair<const vertex_t *, const point_t<D>>(&vertex, point));
        }

        auto insert(const vertex_t & vertex, const point_t<D> && point)
        {
            return _map.insert(
                // std::pair<std::reference_wrapper<vertex_t>, point_t<D>>(vertex, point)
                std::pair<const vertex_t *, const point_t<D>>(&vertex, point));
        }

        const point_t<D> & operator[](const vertex_t * vertex) const
        {
            auto point = _map.find(vertex);
            return point->second;
        }

        const point_t<D> & operator[](const vertex_t & vertex) const
        {
            auto point = _map.find(&vertex);
            return point->second;
        }

      private:
        map_t _map;
    };

}    // namespace mito

#endif    // mito_mesh_VertexSet_h

// end of file
