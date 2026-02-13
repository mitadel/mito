// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // specialization: pullback of ambient metric along a parametrization field
    template <class ambient_metricT>
    struct pullback_metric<ambient_metricT> {
        static constexpr auto field(const fields::field_c auto & parametrization)
        {
            auto g_ambient = ambient_metricT::field();
            auto jacobian = functions::derivative(parametrization);
            return functions::transpose(jacobian) * g_ambient(parametrization) * jacobian;
        }
    };

}


// end of file
