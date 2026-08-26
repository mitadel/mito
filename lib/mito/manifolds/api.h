// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // atlas alias
    template <class cellT, geometry::coordinates_c coordsT>
    using atlas_t = Atlas<cellT, coordsT>;

    // factory of atlases from a coordinate system
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto atlas(const geometry::coordinate_system_t<coordsT> &);

    // manifold element alias
    template <class cellT, class parametrizationT, class metricVolumeFormT>
    using parametrized_element_t = ParametrizedElement<cellT, parametrizationT, metricVolumeFormT>;

    // factory of parametrized elements from a cell, a parametrization, and a metric volume form
    template <class cellT, class parametrizationT, class metricVolumeFormT>
    constexpr auto parametrized_element(
        const cellT & cell, const parametrizationT & parametrization,
        const metricVolumeFormT & metric_volume_form);

    // cell integrator alias
    template <class quadratureRuleT, class parametrizedElementT>
    using cell_integrator_t = CellIntegrator<quadratureRuleT, parametrizedElementT>;

    // cell integrator factory
    template <class quadratureRuleT, class parametrizedElementT>
    constexpr auto cell_integrator(const parametrizedElementT & parametrized_element);

    // manifold elements view alias
    template <class manifoldT>
    using manifold_elements_view_t = ManifoldElementsView<manifoldT>;

    // manifold alias
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    using manifold_t = Manifold<cellT, coordsT, volumeFormT>;

    // factory submanifold from a mesh, a coordinate system and set of normal fields
    template <class cellT, geometry::coordinates_c coordsT, mito::fields::vector_field_c... fieldsT>
    constexpr auto submanifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const fieldsT &... normal_field);
}


// end of file
