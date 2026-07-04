// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    // Class {ParametrizedElement} represents a cell equipped with a parametrization in physical
    // space and a metric volume form.
    template <class cellT, class parametrizationT, class metricVolumeFormT>
    class ParametrizedElement {
      public:
        // the cell type
        using cell_type = cellT;
        // the parametrization type
        using parametrization_type = parametrizationT;
        // the metric volume form type
        using metric_volume_form_type = metricVolumeFormT;

      public:
        // the constructor
        constexpr ParametrizedElement(
            const cell_type & cell, const parametrization_type & parametrization,
            const metric_volume_form_type & metric_volume_form) :
            _cell(cell),
            _parametrization(parametrization),
            _metric_volume_form(metric_volume_form)
        {}

        // destructor
        constexpr ~ParametrizedElement() = default;

        // delete default constructor
        constexpr ParametrizedElement() noexcept = delete;

        // default move constructor
        constexpr ParametrizedElement(ParametrizedElement &&) noexcept = default;

        // default copy constructor
        constexpr ParametrizedElement(const ParametrizedElement &) = default;

        // delete assignment operator
        constexpr ParametrizedElement & operator=(const ParametrizedElement &) = delete;

        // default move assignment operator
        constexpr ParametrizedElement & operator=(ParametrizedElement &&) noexcept = default;

      public:
        // return the underlying cell
        constexpr auto cell() const -> const cell_type & { return _cell; }

        // return the parametrization of this element in physical space
        constexpr auto parametrization() const -> const parametrization_type & { return _parametrization; }

        // return the metric volume form of this element
        constexpr auto metric_volume_form() const -> const metric_volume_form_type &
        {
            return _metric_volume_form;
        }

      private:
        // the underlying cell
        cell_type _cell;
        // the parametrization of the manifold element
        parametrization_type _parametrization;
        // the metric volume form of the manifold element
        metric_volume_form_type _metric_volume_form;
    };

}    // namespace mito


// end of file
