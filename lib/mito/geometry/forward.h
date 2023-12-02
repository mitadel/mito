// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateSystem { EUCLIDEAN, POLAR };

    // class coordinates
    template <int D, CoordinateSystem coordT>
    class Coordinates;

    // class point
    template <int D, CoordinateSystem coordT>
    class Point;

    // class point cloud
    template <int D, CoordinateSystem coordT>
    class PointCloud;

    // class geometry
    template <int D, CoordinateSystem coordT>
    class Geometry;

    // class node
    template <int D, CoordinateSystem coordT>
    class Node;
}


#endif

// end of file
