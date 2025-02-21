// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // addition of a set of coordinates and a vector
    template <coordinates_c coordsT>
    constexpr auto operator+(const coordsT &, const tensor::vector_t<coordsT::dim> &) -> coordsT;

    // subtraction of two sets of coordinates (returns a vector)
    template <coordinates_c coordsT>
    constexpr auto operator-(const coordsT &, const coordsT &) -> tensor::vector_t<coordsT::dim>;

    // operator+=
    template <coordinates_c coordsT>
    constexpr auto operator+=(coordsT & A, const tensor::vector_t<coordsT::dim> & v) -> coordsT &
    {
        A = A + v;
        return A;
    }

    template <coordinates_c coordsT>
    constexpr auto distance(const coordsT & xA, const coordsT & xB) -> tensor::scalar_t
    {
        // return the distance between the two points
        return mito::tensor::norm(xA - xB);
    }

}    // namespace mito


// end of file
