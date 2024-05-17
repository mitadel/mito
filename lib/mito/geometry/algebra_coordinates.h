// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_coordinates_h)
#define mito_geometry_algebra_coordinates_h


namespace mito::geometry {

    // addition of a set of coordinates and a vector
    template <coordinates_c coordsT>
    constexpr auto operator+(const coordsT &, const vector_t<coordsT::dim> &) -> coordsT;

    // subtraction of two sets of coordinates (returns a vector)
    template <coordinates_c coordsT>
    constexpr auto operator-(const coordsT &, const coordsT &) -> vector_t<coordsT::dim>;

    // operator+=
    template <coordinates_c coordsT>
    constexpr auto operator+=(coordsT & A, const vector_t<coordsT::dim> & v) -> coordsT &
    {
        A = A + v;
        return A;
    }

    template <coordinates_c coordsT>
    constexpr auto distance(const coordsT & xA, const coordsT & xB) -> scalar_t
    {
        // return the distance between the two points
        return pyre::tensor::norm(xA - xB);
    }

}    // namespace mito


#endif    // mito_geometry_Coordinates_h

// end of file
