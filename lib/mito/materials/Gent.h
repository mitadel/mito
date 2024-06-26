// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    class Gent {

      public:
        constexpr Gent(real rho, real kappa, real mu, real Jm) :
            _rho(rho),
            _kappa(kappa),
            _mu(mu),
            _Jm(Jm)
        {}

        template <int D /*dim*/>
        constexpr auto Constitutive(const vector_t<D> & u, const matrix_t<D> & Du, matrix_t<D> & P)
            -> void;

      private:
        real _rho;
        real _kappa;
        real _mu;
        real _Jm;
    };

    template <int D /*dim*/>
    constexpr auto Gent::Constitutive(
        const vector_t<D> & /*u*/, const matrix_t<D> & F, matrix_t<D> & P) -> void
    {
        // inverse of deformation gradient
        matrix_t<D> invF = pyre::tensor::inverse(F);

        // J = det F
        real detF = pyre::tensor::determinant(F);

        // precompute useful quantities
        // J^2 - 1
        real Jsq_minus_1 = detF * detF - 1.;
        // log(J)
        real logJ = std::log(detF);
        // tr(C)
        real trC = pyre::tensor::trace(pyre::tensor::transpose(F) * F);
        // (J^2 -1)/2 - log(J)
        real A = 0.5 * Jsq_minus_1 - logJ;
        // Jm - trC + D /*dim*/
        real C = (_Jm - trC + D /*dim*/);
        // Jm / (Jm - trC + D /*dim*/))
        real B = _Jm / C;

        // fill the first Piola stress tensor
        P = (_mu * B) * F
          + (2. * _kappa * A * A * A * Jsq_minus_1 - _mu) * pyre::tensor::transpose(invF);

        return;
    }

}


// end of file
