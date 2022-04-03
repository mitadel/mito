// code guard
#if !defined(mito_mesh_PointCloud_h)
#define mito_mesh_PointCloud_h

// QUESTION: should Point and PointCloud live in the mesh namespace?
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

        // support for ranged for loops (wrapping grid)
        inline const auto begin() const { return _cloud.cbegin(); }
        inline const auto end() const { return _cloud.cend(); }
        inline auto begin() { return _cloud.begin(); }
        inline auto end() { return _cloud.end(); }

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

    // overload operator<< for point clouds
    template <int D>
    std::ostream & operator<<(std::ostream & os, const PointCloud<D> & cloud)
    {
        for (const auto & point : cloud) {
            std::cout << "vertex: " << point.first << ",\t coordinates: " << point.second << std::endl;
        }
        return os;
    }

}    // namespace mito

#endif    // mito_mesh_PointCloud_h

// end of file
