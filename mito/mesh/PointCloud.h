// code guard
#if !defined(mito_mesh_PointCloud_h)
#define mito_mesh_PointCloud_h


namespace mito::mesh {

    template <int D>
    class PointCloud {

        // using cloud_t = std::unordered_map<std::reference_wrapper<vertex_t>, point_t<D>>;
        using cloud_t = std::unordered_map<const vertex_t *, const point_t<D>>;

      public:
        PointCloud() : _cloud() {};

        ~PointCloud() {}

      private:
        // delete copy constructor
        PointCloud(const PointCloud &) = delete;

        // delete move constructor
        PointCloud(const PointCloud &&) = delete;

        // delete assignment operator
        const PointCloud & operator=(const PointCloud &) = delete;

        // delete move assignment operator
        const PointCloud & operator=(const PointCloud &&) = delete;

      public:
        void print() const
        {
            // iterate on map
            for (auto item : _cloud) {
                std::cout << "Vertex: " << item.first << std::endl;
                std::cout << "Point: " << item.second << std::endl;
            }
            // all done
            return;
        }

        int size() { return _cloud.size(); }

        auto insert(const vertex_t & vertex, const point_t<D> & point)
        {
            return _cloud.insert(
                // std::pair<std::reference_wrapper<vertex_t>, point_t<D>>(vertex, point)
                std::pair<const vertex_t *, const point_t<D>>(&vertex, point));
        }

        auto insert(const vertex_t & vertex, const point_t<D> && point)
        {
            return _cloud.insert(
                // std::pair<std::reference_wrapper<vertex_t>, point_t<D>>(vertex, point)
                std::pair<const vertex_t *, const point_t<D>>(&vertex, point));
        }

        const point_t<D> & operator[](const vertex_t * vertex) const
        {
            auto point = _cloud.find(vertex);
            return point->second;
        }

        const point_t<D> & operator[](const vertex_t & vertex) const
        {
            auto point = _cloud.find(&vertex);
            return point->second;
        }

      private:
        cloud_t _cloud;
    };

}    // namespace mito

#endif    // mito_mesh_PointCloud_h

// end of file