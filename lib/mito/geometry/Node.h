// code guard
#if !defined(mito_geometry_Node_h)
#define mito_geometry_Node_h


namespace mito::geometry {

    template <int D>
    class Node {
      public:
        using vertex_t = mito::topology::vertex_t;
        using point_t = mito::geometry::point_t<D>;

      public:
        // get the coordinates of the point
        Node(const vertex_t & vertex, const point_t & point) : _vertex(vertex), _point(point) {}

      public:
        // accessor for the underlying vertex
        auto vertex() -> const vertex_t & { return _vertex; }

        // accessor for the underlying point
        auto point() -> const point_t & { return _point; }

      private:
        // the vertex that this node is attached to
        vertex_t _vertex;
        // the point that this node is attached to
        point_t _point;
    };

}    // namespace mito

#endif    // mito_geometry_Node_h

// end of file
