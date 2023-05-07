// code guard
#if !defined(mito_geometry_PointCloud_h)
#define mito_geometry_PointCloud_h

namespace mito::geometry {

    template <int D>
    class PointCloud {
      private:
        using cloud_t = mito::geometry::cloud_t<D>;
        // TOFIX: make this unordered?
        using point_compositions_t = std::map<vector_t<D>, point_t<D>>;

      private:
        PointCloud() : _cloud(100 /*segment size */), _compositions() {}

        // delete copy constructor
        PointCloud(const PointCloud<D> &) = delete;

        // delete assignment operator
        void operator=(const PointCloud<D> &) = delete;

        // destructor
        ~PointCloud() {};

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
        auto point(vector_t<D> && coord) -> const point_t<D> &
        {
            // helper function to convert vector_t to variadic template argument
            auto _emplace_point = [this]<size_t... I>(
                                      const vector_t<D> & coord,
                                      std::index_sequence<I...>) -> point_t<D> {
                // get from {_cloud} the location where to place the new point
                auto location = _cloud.location_for_placement();

                // create a new point at location {location} with placement new
                Point<D> * resource = new (location) Point<D>(coord[I]...);

                // wrap the new point in a shared pointer and return it
                return mito::utilities::shared_ptr<Point<D>>(resource, &_cloud);
            };

            // emplace point in {_cloud}
            auto point = _emplace_point(coord, std::make_index_sequence<D> {});

            // register it in the compositions map
            auto it = _compositions.insert(std::make_pair(point->coordinates(), point));

            // return the newly added point
            return it.first->second;
        }

      private:
        // the cloud of points
        cloud_t _cloud;

        // the container mapping the composition of points to the points themselves
        point_compositions_t _compositions;

        // friendship with the singleton
        using PointCloudSingleton = mito::utilities::Singleton<PointCloud<D>>;
        friend PointCloudSingleton;
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
