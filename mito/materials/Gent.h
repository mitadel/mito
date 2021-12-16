#if !defined(mito_materials_Gent_h)
#define mito_materials_Gent_h

#include "../mito.h"

namespace mito {

    class Gent {

      public:
        Gent(real rho, real kappa, real mu, real Jm) :
            _rho(rho),
            _kappa(kappa),
            _mu(mu),
            _Jm(Jm) {};

        template <int D /*dim*/>
        void Constitutive(const vector_t<D> & u, const matrix_t<D> & Du, matrix_t<D> & P);

      private:
        real _rho;
        real _kappa;
        real _mu;
        real _Jm;
    };

    template <int D /*dim*/>
    void Gent::Constitutive(const vector_t<D> & u, const matrix_t<D> & Du, matrix_t<D> & P)
    {
        // deformation gradient
        matrix_t<D> F = Du;

        // inverse of deformation gradient
        matrix_t<D> invF = pyre::algebra::inverse(F);

        // J = det F
        real detF = pyre::algebra::determinant(F);

        // precompute useful quantities
        // J^2 - 1
        real Jsq_minus_1 = detF * detF - 1.;
        // log(J)
        real logJ = log(detF);
        // tr(C)
        real trC = 0.0;
        for (int i = 0; i < D; i++) {
            for (int J = 0; J < D; J++) {
                trC += F[i * D + J] * F[i * D + J];
            }
        }
        // (J^2 -1)/2 - log(J)
        real A = 0.5 * Jsq_minus_1 - logJ;
        // Jm - trC + D /*dim*/
        real C = (_Jm - trC + D /*dim*/);
        // Jm / (Jm - trC + D /*dim*/))
        real B = _Jm / C;

        // fill the first Piola stress tensor
        for (int i = 0; i < D; i++) {
            for (int J = 0; J < D; J++) {
                P[i * D + J] = _mu * B * F[i * D + J]
                             + (2. * _kappa * A * A * A * Jsq_minus_1 - _mu) * invF[J * D + i];
            }
        }

        return;
    }

}

#endif    // mito_materials_Gent_h

// end of file
