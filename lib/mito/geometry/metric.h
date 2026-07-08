// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // the Euclidean metric tensor field in {coordsT} coordinates
    template <coordinates_c coordsT>
    struct euclidean_metric {};

    // the pullback of ambient metric along a manifold parametrized by {parametrization}
    template <class ambient_metricT>
    struct pullback_metric {
        static constexpr auto field(const fields::field_c auto & parametrization)
        {
            // fetch the ambient metric tensor field
            auto g_ambient = ambient_metricT::field();
            // compute the jacobian of the parametrization field, which maps vectors from the
            // tangent space of the submanifold to the tangent space of the ambient manifold
            auto jacobian = functions::derivative(parametrization);
            // assemble the pullback metric tensor field
            return functions::transpose(jacobian) * g_ambient(parametrization) * jacobian;
        }
    };

}


// end of file
