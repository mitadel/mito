// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_PointCloud_h)
#define mito_geometry_PointCloud_h


namespace mito::geometry {

    template <int D>
    class PointCloud {
      private:
        // a point
        using point_type = point_t<D>;
        // a cloud of points
        using cloud_type = utilities::repository_t<point_type>;

      private:
        PointCloud() : _cloud(100 /*segment size */) {}

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
            for (const auto & point : _cloud) {
                std::cout << point.id() << std::endl;
            }
            // all done
            return;
        }

        // support for ranged for loops (wrapping grid)
        inline auto begin() const -> const auto & { return std::cbegin(_cloud); }
        inline auto end() const -> const auto & { return std::cend(_cloud); }

        auto size() const noexcept -> int { return std::size(_cloud); }

        // instantiate new point
        auto point() -> point_type
        {
            // return the newly added point
            return _cloud.emplace();
        }

        // the iterable repository of the points in the cloud
        auto points() const noexcept -> const cloud_type & { return _cloud; }

      private:
        // the cloud of points
        cloud_type _cloud;

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
