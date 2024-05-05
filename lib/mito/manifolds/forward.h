// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class metric space
    template <geometry::coordinates_c coordsT>
    class MetricSpace;

    // class manifold
    template <class cellT, geometry::coordinates_c coordsT>
    requires(cellT::dim == coordsT::dim)
    class Manifold;

    // class manifold
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    requires(cellT::dim == coordsT::dim)
    class Submanifold;
}


#endif

// end of file
