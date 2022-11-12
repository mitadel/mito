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

        // example use: cloud.point({0.0, ..., 0.0})
        auto point(vector_t<D> && coord) -> auto
        {
            // helper function to convert vector_t to variadic template argument
            auto _emplace_point = [this]<size_t... I>(
                                      const vector_t<D> & coord, std::index_sequence<I...>)
                                      ->auto
            {
                // return the newly added point
                return _cloud.emplace(coord[I]...);
            };
            // return the newly added point
            return _emplace_point(coord, std::make_index_sequence<D> {});
        }

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
