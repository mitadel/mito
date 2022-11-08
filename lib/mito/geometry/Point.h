// code guard
#if !defined(mito_geometry_Point_h)
#define mito_geometry_Point_h


namespace mito::geometry {

    template <int D>
    class Point : public vector_t<D>, public mito::utilities::Shareable {
      public:
        template <class... Args>
        Point(Args &&... args)
        requires(sizeof...(Args) == D)
            : vector_t<D>(args...), mito::utilities::Shareable()
        {}
    };

}    // namespace mito

#endif    // mito_geometry_Point_h

// end of file
