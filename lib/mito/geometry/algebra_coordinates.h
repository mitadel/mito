// code guard
#if !defined(mito_geometry_algebra_coordinates_h)
#define mito_geometry_algebra_coordinates_h


namespace mito::geometry {

    // subtraction of coordinates xA - xB
    template <int D, CoordinateSystem coordT>
    constexpr auto operator-(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> Coordinates<D, coordT>
    {
        return xA + (-1.0) * xB;
    }

    // multiplication of coordinates xA * b
    template <int D, CoordinateSystem coordT>
    constexpr auto operator*(const Coordinates<D, coordT> & xA, const scalar_t & b)
        -> Coordinates<D, coordT>
    {
        return b * xA;
    }

    // division of coordinates xA / b
    template <int D, CoordinateSystem coordT>
    constexpr auto operator/(const Coordinates<D, coordT> & xA, const scalar_t & b)
        -> Coordinates<D, coordT>
    {
        return (1.0 / b) * xA;
    }

    // operator==
    template <int D, CoordinateSystem coordT>
    constexpr auto operator==(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> bool
    {
        return xA._array == xB._array;
    }

    template <int D, CoordinateSystem coordT>
    constexpr auto distance(
        const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB) noexcept -> scalar_t
    {
        // return the distance between the two points
        return length(xA - xB);
    }

}    // namespace mito


// implementation of custom {operator+}, {operator*}, and {length}
#include "algebra_euclidean_coordinates.icc"
#include "algebra_polar_coordinates.icc"

#endif    // mito_geometry_Coordinates_h

// end of file
