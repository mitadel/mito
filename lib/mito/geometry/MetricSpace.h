// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_MetricSpace_h)
#define mito_geometry_MetricSpace_h


namespace mito::geometry {

    template <coordinates_c coordsT>
    class MetricSpace {

      private:
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = coordsT::dim;

      public:
        // the dimension of the physical space
        static constexpr int dim = D;
        // typedef for a set of coordinates
        using coordinates_type = coordsT;

      public:
        // get the I-th basis element for vector fields
        template <int I>
        constexpr auto e() const;

        // get metric
        constexpr auto g() const;

        // get the I-th basis element for one-form fields
        template <int I>
        constexpr auto dx() const;

        // get the metric volume form
        constexpr auto w() const;

        // get the metric equivalent vector field to a given one-form field
        constexpr auto metric_equivalent_vector(
            const fields::one_form_field_c auto & one_form) const;

        // get the metric equivalent form field to a given vector field
        constexpr auto metric_equivalent_form(const fields::vector_field_c auto & vector) const;

      private:
        // get the one form representing the contraction with {matrix} and {vector}
        template <
            fields::vector_field_c vectorFieldT, fields::symmetric_tensor_field_c tensorFieldT>
        static constexpr auto _one_form(const vectorFieldT & vector, const tensorFieldT & matrix)
        requires(fields::compatible_fields_c<vectorFieldT, tensorFieldT>);

      private:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto _e = Basis<coordinates_type>::template e<I>();

        // the metric field in coordinates {coordinates_type}
        static constexpr auto _g = metric<coordinates_type>::field();

        // the inverse metric field in coordinates {coordinates_type}
        static constexpr auto _g_inv = fields::inverse(_g);

        // the dual basis for one-form fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto _dx = _one_form(_e<I>, _g_inv);

        // helper function wedging the N basis 1-forms
        template <int... J>
        static constexpr auto _wedge(integer_sequence<J...>)
        requires(sizeof...(J) == D)
        {
            // return the basis N-form
            return fields::wedge(_dx<J>...);
        }

        // the metric volume form
        static constexpr auto _w =
            fields::sqrt(fields::determinant(_g)) * _wedge(make_integer_sequence<D>{});
    };

}    // namespace mito


// get the inline definitions
#define mito_geometry_metric_space_icc
#include "MetricSpace.icc"
#undef mito_geometry_metric_space_icc


#endif

// end of file
