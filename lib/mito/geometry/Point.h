// code guard
#if !defined(mito_geometry_Point_h)
#define mito_geometry_Point_h


namespace mito::geometry {

    template <int D>
    class Point : public mito::utilities::Shareable {
      public:    // TOFIX: should be private (do not allow to instantiate points outside of a point
                 // cloud)
        template <class... Args>
        Point(Args &&... args)
        requires(sizeof...(Args) == D)
            : _coordinates(args...)
        {}

      public:
        // get the coordinates of the point
        auto coordinates() const -> const vector_t<D> & { return _coordinates; }

      private:
        vector_t<D> _coordinates;
    };

    template <int D>
    auto distance(const point_t<D> & pointA, const point_t<D> & pointB) -> real
    {
        // return the distance between the two points
        auto dist = pointA->coordinates() - pointB->coordinates();
        return sqrt(dist * dist);
    }

}    // namespace mito

#endif    // mito_geometry_Point_h

// end of file
