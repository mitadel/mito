// code guard
#if !defined(mito_geometry_Node_h)
#define mito_geometry_Node_h


namespace mito::geometry {

    template <int D>
    class Node {
      public:
        using vertex_type = topology::vertex_t;
        using point_type = point_t<D>;

      public:
        // get the coordinates of the point
        Node(const vertex_type & vertex, const point_type & point) : _vertex(vertex), _point(point)
        {}

      public:
        // accessor for the underlying vertex
        auto vertex() const noexcept -> const vertex_type & { return _vertex; }

        // accessor for the underlying point
        auto point() const noexcept -> const point_type & { return _point; }

      private:
        // the vertex that this node is attached to
        vertex_type _vertex;
        // the point that this node is attached to
        point_type _point;
    };

    template <int D>
    inline bool operator==(const Node<D> & lhs, const Node<D> & rhs) noexcept
    {
        return lhs.vertex() == rhs.vertex() && lhs.point() == rhs.point();
    }

}    // namespace mito

#endif    // mito_geometry_Node_h

// end of file
