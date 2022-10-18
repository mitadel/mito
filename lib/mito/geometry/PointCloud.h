// code guard
#if !defined(mito_geometry_PointCloud_h)
#define mito_geometry_PointCloud_h

namespace mito::geometry {

    template <int D>
    class PointCloud {
      private:
        using cloud_t = mito::geometry::cloud_t<D>;

      public:
        PointCloud() = delete;

      public:
        static auto print() -> void
        {
            // iterate on map
            for (auto item : _cloud) {
                std::cout << "Vertex: " << item.first->id() << std::endl;
                std::cout << "Point: " << item.second << std::endl;
            }
            // all done
            return;
        }

        // support for ranged for loops (wrapping grid)
        static inline auto begin() -> const auto & { return _cloud.cbegin(); }
        static inline auto end() -> const auto & { return _cloud.cend(); }

        static auto size() -> int { return _cloud.size(); }

        static auto insert(const vertex_t & vertex, const point_t<D> & point) -> auto
        {
            return _cloud.insert(std::pair<vertex_t, const point_t<D>>(vertex, point));
        }

        static auto insert(const vertex_t & vertex, const point_t<D> && point) -> auto
        {
            return _cloud.insert(std::pair<vertex_t, const point_t<D>>(vertex, point));
        }

        // TODO: accessor operator[](point_t) -> a list of all vertices sitting on the same point
        static auto point(const vertex_t & vertex) -> const point_t<D> &
        {
            return _cloud.find(vertex)->second;
        }

      private:
        static cloud_t _cloud;
    };

    // initialize static attribute
    template <int D>
    typename PointCloud<D>::cloud_t PointCloud<D>::_cloud = PointCloud<D>::cloud_t();

    template <int D>
    std::ostream & operator<<(std::ostream & os, const PointCloud<D> & cloud)
    {
        for (const auto & point : cloud) {
            std::cout << "vertex: " << point.first->id() << ",\t coordinates: " << point.second
                      << std::endl;
        }
        return os;
    }

}    // namespace mito

#endif    // mito_geometry_PointCloud_h

// end of file
