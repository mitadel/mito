// code guard
#if !defined(mito_geometry_Point_h)
#define mito_geometry_Point_h


namespace mito::geometry {

    template <int D>
    class Point : public utilities::Shareable {
      private:
        template <class... Args>
        constexpr Point(Args &&... args)
        requires(sizeof...(Args) == D)
            : _coordinates(args...)
        {}

        constexpr ~Point() override {}

        // delete copy constructor
        Point(const Point &) = delete;

        // delete move constructor
        Point(Point &&) = delete;

        // delete assignment operator
        Point & operator=(const Point &) = delete;

        // delete move assignment operator
        Point & operator=(Point &&) = delete;

      public:
        // get the coordinates of the point
        constexpr auto coordinates() const noexcept -> const vector_t<D> & { return _coordinates; }

        auto print() const noexcept -> void
        {
            // print the coordinates of the point
            std::cout << "Point: " << coordinates() << std::endl;

            // all done
            return;
        }

      private:
        // the coordinates of the point
        const vector_t<D> _coordinates;

        // private friendship with the point cloud
        friend class PointCloud<D>;
    };

    template <int D>
    constexpr auto distance(const point_t<D> & pointA, const point_t<D> & pointB) noexcept -> real
    {
        // return the distance between the two points
        auto dist = pointA->coordinates() - pointB->coordinates();
        return std::sqrt(dist * dist);
    }

    template <int D>
    std::ostream & operator<<(std::ostream & os, const Point<D> & point) noexcept
    {
        // print the point
        point.print();

        // all done
        return os;
    }

}    // namespace mito

#endif    // mito_geometry_Point_h

// end of file
