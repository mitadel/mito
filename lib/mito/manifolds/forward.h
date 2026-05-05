// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // class atlas
    template <class cellT, geometry::coordinates_c coordsT>
    requires(cellT::dim == coordsT::dim)
    class Atlas;

    // class parametrized element
    template <class cellT, class parametrizationT, class metricVolumeFormT>
    class ParametrizedElement;

    // class manifold elements view
    template <class manifoldT>
    class ManifoldElementsView;

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
