// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <coordinates_c coordsT>
    struct metric_space {

      private:
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = coordsT::dim;

        // typedef for a set of coordinates
        using coordinates_type = coordsT;

      private:
        // get the one form representing the contraction with {matrix} and {vector}
        template <
            fields::vector_field_c vectorFieldT, fields::symmetric_tensor_field_c tensorFieldT>
        static constexpr auto _one_form(const vectorFieldT & vector, const tensorFieldT & matrix)
        requires(fields::compatible_fields_c<vectorFieldT, tensorFieldT>);

      public:
        // get the I-th coordinate basis element for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e = basis<coordinates_type>::template e<I>();

        // the metric field in coordinates {coordinates_type}
        static constexpr auto g = metric<coordinates_type>::field();

        // the inverse metric field in coordinates {coordinates_type}
        static constexpr auto g_inv = functions::inverse(g);

        // get the I-th basis element for one-form fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto dx = _one_form(e<I>, g_inv);

      private:
        // helper function wedging the N basis 1-forms
        template <int... J>
        static constexpr auto _wedge(tensor::integer_sequence<J...>)
        requires(sizeof...(J) == D)
        {
            // return the basis N-form
            return fields::wedge(dx<J>...);
        }

      public:
        // the metric volume form
        static constexpr auto w =
            functions::sqrt(fields::determinant(g)) * _wedge(tensor::make_integer_sequence<D>{});

        // get the metric equivalent vector field to a given one-form field
        static constexpr auto metric_equivalent(const fields::one_form_field_c auto & one_form);

        // get the metric equivalent form field to a given vector field
        static constexpr auto metric_equivalent(const fields::vector_field_c auto & vector);
    };


    template <coordinates_c coordsT>
    constexpr auto metric_space<coordsT>::metric_equivalent(
        const fields::one_form_field_c auto & one_form)
    {
        // return a vector field that, once evaluated at {x}...
        return functions::function([one_form](const coordinates_type & x) {
            // returns the contraction of the inverse metric with the components of the one
            // form
            auto _one_form_components = [one_form, x]<int... K>(tensor::integer_sequence<K...>) {
                return ((one_form(x)(e<K>(x)) * g_inv(x) * e<K>(x)) + ...);
            };

            // all done
            return _one_form_components(tensor::make_integer_sequence<D>{});
        });
    }

    template <coordinates_c coordsT>
    constexpr auto metric_space<coordsT>::metric_equivalent(
        const fields::vector_field_c auto & vector)
    {
        // return a one form field that, once evaluated at {x}...
        return _one_form(vector, g_inv);
    }

    template <coordinates_c coordsT>
    template <fields::vector_field_c vectorFieldT, fields::symmetric_tensor_field_c tensorFieldT>
    constexpr auto metric_space<coordsT>::_one_form(
        const vectorFieldT & vector, const tensorFieldT & matrix)
    requires(fields::compatible_fields_c<vectorFieldT, tensorFieldT>)
    {
        // return a one form field that, once evaluated at {x}...
        return functions::function([vector, matrix](const coordinates_type & x) {
            // returns the metric equivalent form to {vector(x)}
            return tensor::one_form(vector(x), matrix(x));
        });
    }
}    // namespace mito


// end of file
