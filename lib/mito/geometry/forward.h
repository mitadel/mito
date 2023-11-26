// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateSystem { EUCLIDEAN, POLAR };

    // class coordinates
    template <int D, CoordinateSystem coordT = EUCLIDEAN>
    class Coordinates;

    // class point
    template <int D>
    class Point;

    // class point cloud
    template <int D>
    class PointCloud;

    // class geometry
    template <int D>
    class Geometry;

    // class node
    template <int D>
    class Node;

}


#endif

// end of file
