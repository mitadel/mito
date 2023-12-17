// code guard
#if !defined(mito_geometry_forward_h)
#define mito_geometry_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateType { EUCLIDEAN, POLAR };

    // class coordinates
    template <int D, CoordinateType coordT>
    class Coordinates;

    // class coordinate system
    template <int D, CoordinateType coordT>
    class CoordinateSystem;

    // class point
    template <int D>
    class Point;

    // class geometric simplex
    template <int N, int D>
    class GeometricSimplex;

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
