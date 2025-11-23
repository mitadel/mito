// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // class manifold
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    requires(cellT::dim == coordsT::dim)
    class Manifold;

    // concept of a manifold
    template <class F>
    concept manifold_c = requires(F c) {
        // require that F only binds to {Manifold} specializations
        []<class cellT, geometry::coordinates_c coordsT, class volumeFormT>(
            const Manifold<cellT, coordsT, volumeFormT> &) {
        }(c);
    };
}


// end of file
