// code guard
#if !defined(mito_geometry_Point_h)
#define mito_geometry_Point_h


namespace mito::geometry {

    template <int D>
    class Point : public utilities::Shareable {
      private:
        using coordinates_type = coordinates_t<D>;

      private:
        constexpr Point(const coordinates_type & coordinates) : _coordinates(coordinates) {}

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
        constexpr auto coordinates() const noexcept -> const coordinates_type &
        {
            return _coordinates;
        }

        auto print() const noexcept -> void
        {
            // print the coordinates of the point
            std::cout << "Point: " << coordinates() << std::endl;

            // all done
            return;
        }

      private:
        // the coordinates of the point
        const coordinates_type _coordinates;

        // private friendship with the repository of points
        friend class utilities::Repository<point_t<D>>;
    };

    template <int D>
    constexpr auto distance(const point_t<D> & pointA, const point_t<D> & pointB) noexcept -> real
    {
        // return the distance between the two points
        auto dist = pointA->coordinates() - pointB->coordinates();
        return std::sqrt(dist * dist);
    }

    template <int D>
    std::ostream & operator<<(std::ostream & os, const point_t<D> & point) noexcept
    {
        // print the point
        point->print();

        // all done
        return os;
    }

}    // namespace mito


#endif    // mito_geometry_Point_h

// end of file
