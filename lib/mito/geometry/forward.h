// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateType { EUCLIDEAN, POLAR };

    // class coordinates
    template <int D, CoordinateType coordT>
    class Coordinates;

    // class point
    template <int D, CoordinateType coordT>
    class Point;

    // class point cloud
    template <int D, CoordinateType coordT>
    class PointCloud;

    // class geometry
    template <int D, CoordinateType coordT>
    class Geometry;

    // class node
    template <int D, CoordinateType coordT>
    class Node;
}


#endif

// end of file
