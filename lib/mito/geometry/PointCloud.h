// code guard
#if !defined(mito_geometry_PointCloud_h)
#define mito_geometry_PointCloud_h

namespace mito::geometry {

    template <int D>
    class PointCloud {
      private:
        using cloud_t = geometry::cloud_t<D>;
        // id type of point
        using point_id_t = utilities::index_t<point_t<D>>;
        // TOFIX: make this unordered?
        using point_compositions_t = std::map<vector_t<D>, point_id_t>;

      private:
        PointCloud() : _cloud(100 /*segment size */), _compositions() {}

        // delete copy constructor
        PointCloud(const PointCloud<D> &) = delete;

        // delete assignment operator
        void operator=(const PointCloud<D> &) = delete;

        // destructor
        ~PointCloud() {}

      public:
        auto print() const noexcept -> void
        {
            // iterate on points
            std::cout << "Point cloud:" << std::endl;
            for (const auto & pPointMap : _compositions) {
                std::cout << _cloud.resource(pPointMap.second)->coordinates() << std::endl;
            }
            // all done
            return;
        }

        // support for ranged for loops (wrapping grid)
        // TOFIX: define iterators on {Repository} so we can return simply
        // std::cbegin(_cloud), ...
        inline auto begin() -> const auto & { return std::cbegin(_cloud.resources()); }
        inline auto end() -> const auto & { return std::cend(_cloud.resources()); }

        auto size() const noexcept -> int { return std::size(_cloud.resources()); }

        // example use: cloud.point({0.0, ..., 0.0})
        auto point(vector_t<D> && coord) -> point_t<D>
        {
            // helper function to convert vector_t to variadic template argument
            auto _emplace_point = [this]<size_t... I>(
                                      const vector_t<D> & coord, std::index_sequence<I...>)
                                      ->point_t<D>
            {
                // emplace a new point in the cloud and return it
                return _cloud.emplace(coord[I]...);
            };

            // emplace point in {_cloud}
            auto point = _emplace_point(coord, std::make_index_sequence<D> {});

            // register it in the compositions map
            _compositions.insert(std::make_pair(point->coordinates(), point.id()));

            // return the newly added point
            return point;
        }

        auto compositions() const noexcept -> const point_compositions_t & { return _compositions; }

      private:
        // the cloud of points
        cloud_t _cloud;

        // TOFIX: perhaps this container is unused?
        // the container mapping the composition of points to the points themselves
        point_compositions_t _compositions;

        // friendship with the singleton
        using PointCloudSingleton = utilities::Singleton<PointCloud<D>>;
        friend PointCloudSingleton;
    };

    template <int D>
    std::ostream & operator<<(std::ostream & os, const PointCloud<D> & cloud) noexcept
    {
        // print the cloud
        cloud.print();

        // all done
        return os;
    }

}    // namespace mito

#endif    // mito_geometry_PointCloud_h

// end of file
