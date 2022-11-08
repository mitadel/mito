// code guard
#if !defined(mito_geometry_PointCloud_h)
#define mito_geometry_PointCloud_h

namespace mito::geometry {

    template <int D>
    class PointCloud {
      private:
        using cloud_t = mito::geometry::cloud_t<D>;

      public:
        PointCloud() : _cloud() {}

      public:
        auto print() const -> void
        {
            // iterate on map
            for (auto item : _cloud) {
                std::cout << "Point: " << *item << std::endl;
            }
            // all done
            return;
        }

        // support for ranged for loops (wrapping grid)
        inline auto begin() -> const auto & { return _cloud.cbegin(); }
        inline auto end() -> const auto & { return _cloud.cend(); }

        auto size() -> int { return _cloud.size(); }

        template <class... Args>
        auto point(Args &&... args) -> auto
        requires(sizeof...(Args) == D)
        {
            return _cloud.emplace(std::forward<Args>(args)...);
        }

        // TOFIX: this belongs to the mesh
        // auto insert(const vertex_t & vertex, const point_t<D> & point) -> auto
        // {
        //     return _cloud.insert(std::pair<vertex_t, const point_t<D>>(vertex, point));
        // }

        // TOFIX: this belongs to the mesh
        // auto insert(const vertex_t & vertex, const point_t<D> && point) -> auto
        // {
        //     return _cloud.insert(std::pair<vertex_t, const point_t<D>>(vertex, point));
        // }

        // TOFIX: this belongs to the mesh
        // TODO: accessor operator[](point_t) -> a list of all vertices sitting on the same point
        // auto point(const vertex_t & vertex) -> const point_t<D> &
        // {
        //     return _cloud.find(vertex)->second;
        // }

      private:
        cloud_t _cloud;
    };

    template <int D>
    std::ostream & operator<<(std::ostream & os, const PointCloud<D> & cloud)
    {
        // print the cloud
        cloud.print();

        // all done
        return os;
    }

}    // namespace mito

#endif    // mito_geometry_PointCloud_h

// end of file
