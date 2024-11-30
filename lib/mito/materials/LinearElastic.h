// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    class LinearElastic {

      public:
        constexpr LinearElastic(real rho, real E, real nu) :
            _rho(rho),
            _E(E),
            _nu(nu),
            _lambda(_E * _nu / ((1.0 + _nu) * (1.0 - 2.0 * _nu))),
            _mu(_E / (2.0 * (1.0 + _nu)))
        {}

        constexpr auto Constitutive(
            const vector_c auto & u, const matrix_c auto & Du, matrix_c auto & P,
            fourth_order_tensor_c auto & C) const -> void;

      private:
        // density
        real _rho;
        // Young's modulus
        real _E;
        // Poisson's ratio
        real _nu;
        // 1st Lamé parameter
        real _lambda;
        // 2nd Lamé parameter
        real _mu;
    };

    constexpr auto LinearElastic::Constitutive(
        const vector_c auto & u, const matrix_c auto & Du, matrix_c auto & P,
        fourth_order_tensor_c auto & C) const -> void
    {
        // get the dimension of the physical space
        constexpr int D = utilities::base_type<decltype(u)>::size;

        // the linear strain associated with {Du}
        auto epsilon = symmetric(Du - identity<D>);

        // trace of epsilon
        real tr = trace(epsilon);

        // fill the stress tensor
        P = _lambda * tr * identity<D> + 2.0 * _mu * epsilon;

        // fill the tangent tensor
        C = fourth_order_tensor_t<D>();
        for (int a = 0; a < D; ++a) {
            for (int b = 0; b < D; ++b) {
                C[{ a, a, b, b }] += _lambda;
                C[{ a, b, a, b }] += _mu;
                C[{ b, a, a, b }] += _mu;
            }
        }

        // all done
        return;
    }
}


// end of file
