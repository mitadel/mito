// code guard
#if !defined(mito_geometry_Point_h)
#define mito_geometry_Point_h


namespace mito::geometry {

    template <int D, CoordinateSystem coordT>
    class Point : public utilities::Shareable {
      private:
        // alias for a set of coordinates
        using coordinates_type = coordinates_t<D, coordT>;

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
        friend class utilities::Repository<point_t<D, coordT>>;
    };

    template <int D, CoordinateSystem coordT>
    constexpr auto distance(const point_t<D, coordT> & pointA, const point_t<D, coordT> & pointB)
        -> real
    {
        // return the distance between the two points
        return distance(pointA->coordinates(), pointB->coordinates());
    }

    template <int D, CoordinateSystem coordT>
    std::ostream & operator<<(std::ostream & os, const point_t<D, coordT> & point) noexcept
    {
        // print the point
        point->print();

        // all done
        return os;
    }

}    // namespace mito


#endif    // mito_geometry_Point_h

// end of file
