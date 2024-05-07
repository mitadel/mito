// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_MetricSpace_h)
#define mito_manifolds_MetricSpace_h


namespace mito::manifolds {

    template <geometry::coordinates_c coordsT>
    class MetricSpace {

      private:
        // the physical dimension of the manifold (that is that of the cell)
        static constexpr int D = coordsT::dim;

      public:
        // the dimension of the physical space
        static constexpr int dim = D;
        // typedef for a set of coordinates
        using coordinates_type = coordsT;

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
        static constexpr auto _dx = fields::one_form_field(_e<I>, _g_inv);

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

      public:
        // get the I-th basis element for vector fields
        template <int I>
        constexpr auto e() const
        {
            // all done
            return _e<I>;
        }

        // get metric
        constexpr auto g() const
        {
            // all done
            return _g;
        }

        // get the I-th basis element for one-form fields
        template <int I>
        constexpr auto dx() const
        {
            // all done
            return _dx<I>;
        }

        // get the metric volume form
        constexpr auto w() const
        {
            // all done
            return _w;
        }

        // get the metric equivalent vector field to a given one-form field
        constexpr auto metric_equivalent_vector(
            const fields::one_form_field_c auto & one_form) const
        {
            // return a vector field that, once evaluated at {x}...
            return fields::field(functions::function([one_form, *this](const coordinates_type & x) {
                // returns the contraction of the inverse metric with the components of the one form
                auto _one_form_components = [one_form, x]<int... K>(integer_sequence<K...>) {
                    return ((one_form(x)(_e<K>(x)) * _g_inv(x) * _e<K>(x)) + ...);
                };

                // all done
                return _one_form_components(make_integer_sequence<D>{});
            }));
        }
    };

}    // namespace mito


#endif

// end of file
