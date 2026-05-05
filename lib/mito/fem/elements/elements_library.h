// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES:
// This file provides the main user-facing API for selecting finite elements.
// Users specify only the polynomial degree and manifold type:
//
//   using element_t = mito::fem::isoparametric_simplex_t<degree, ManifoldT>;
//
// The type system automatically extracts:
//   - Coordinate type (coordsT) from ManifoldT::coordinates_type
//   - Volume form type (VolumeFormT) from ManifoldT::volume_form_type
//   - Cell dimensionality to select segment vs triangle


#include "seg1/public.h"
#include "tri1/public.h"
#include "tri2/public.h"


namespace mito::fem {

    template <int degree, class ManifoldT>
    struct isoparametric_simplex;

    // specialization for degree 1, 1D manifolds (segments)
    template <class ManifoldT>
    requires(ManifoldT::cell_type::order == 1)    // 1D cell (segment)
    struct isoparametric_simplex<1, ManifoldT> {
        using coordinates_type = typename ManifoldT::coordinates_type;
        using volume_form_type = typename ManifoldT::volume_form_type;

        using type = IsoparametricSegmentP1<coordinates_type, volume_form_type>;
    };

    // specialization for degree 1, 2D manifolds (triangles)
    template <class ManifoldT>
    requires(ManifoldT::cell_type::order == 2)    // 2D cell (triangle)
    struct isoparametric_simplex<1, ManifoldT> {
        using coordinates_type = typename ManifoldT::coordinates_type;
        using volume_form_type = typename ManifoldT::volume_form_type;

        using type = IsoparametricTriangleP1<coordinates_type, volume_form_type>;
    };

    // specialization for degree 2, 2D manifolds (triangles)
    template <class ManifoldT>
    requires(ManifoldT::cell_type::order == 2)    // 2D cell (triangle)
    struct isoparametric_simplex<2, ManifoldT> {
        using coordinates_type = typename ManifoldT::coordinates_type;
        using volume_form_type = typename ManifoldT::volume_form_type;

        using type = IsoparametricTriangleP2<coordinates_type, volume_form_type>;
    };

    // convenience alias - primary way users specify element types
    // usage: isoparametric_simplex_t<degree, decltype(manifold)>
    template <int degree, class ManifoldT>
    using isoparametric_simplex_t = typename isoparametric_simplex<degree, ManifoldT>::type;

}    // namespace mito::fem


// end of file
