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
            : _coodinates(args...)
        {}

      public:
        // get the coordinates of the point
        auto coordinates() const -> const vector_t<D> & { return _coodinates; }

      private:
        vector_t<D> _coodinates;
    };

}    // namespace mito

#endif    // mito_geometry_Point_h

// end of file
