// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// a compressible Gent material model
class mito::materials::Gent {
  private:
    // the dimension of the physical space
    static constexpr int D = 3;
    // the type for a scalar
    using scalar_type = scalar_t;
    // the type for the deformation gradient
    using deformation_gradient_type = matrix_t<D>;
    // the type for the stress
    using stress_type = matrix_t<D>;
    // the type for the tangents
    using tangents_type = fourth_order_tensor_t<D>;

  public:
    // constructor
    constexpr Gent(real rho, real kappa, real mu, real Jm) :
        _rho(rho),
        _kappa(kappa),
        _mu(mu),
        _Jm(Jm)
    {}

  public:
    // returns the strain energy density
    constexpr auto energy(const deformation_gradient_type &) const -> scalar_type;

    // returns the stress tensor
    constexpr auto stress(const deformation_gradient_type &) const -> stress_type;

    // returns the material tangents
    constexpr auto tangents(const deformation_gradient_type &) const -> tangents_type;

  private:
    // density
    scalar_type _rho;
    // bulk modulus
    scalar_type _kappa;
    // shear modulus
    scalar_type _mu;
    // Jm = Im − 1, with Im being the maximum value for the first invariant I1
    scalar_type _Jm;
};

constexpr auto
mito::materials::Gent::energy(const deformation_gradient_type & F) const -> scalar_type
{
    // precompute useful quantities
    // J = det F
    auto detF = determinant(F);
    // J^2 - 1
    auto Jsq_minus_1 = detF * detF - 1.;
    // log(J)
    auto logJ = std::log(detF);
    // tr(C)
    auto trC = trace(transpose(F) * F);
    // (J^2 -1)/2 - log(J)
    auto A = 0.5 * Jsq_minus_1 - logJ;

    // return the strain energy density
    return -0.5 * _mu * (_Jm * std::log(1. - (trC - 3.) / _Jm) + 2. * logJ)
         + 0.5 * _kappa * A * A * A * A;
}

constexpr auto
mito::materials::Gent::stress(const deformation_gradient_type & F) const -> stress_type
{
    // precompute useful quantities
    // inverse of deformation gradient
    auto invF = inverse(F);
    // J = det F
    auto detF = determinant(F);
    // J^2 - 1
    auto Jsq_minus_1 = detF * detF - 1.;
    // log(J)
    auto logJ = std::log(detF);
    // tr(C)
    auto trC = trace(transpose(F) * F);
    // (J^2 -1)/2 - log(J)
    auto A = 0.5 * Jsq_minus_1 - logJ;
    // Jm - trC + D /*dim*/
    auto C = (_Jm - trC + D /*dim*/);
    // Jm / (Jm - trC + D /*dim*/))
    auto B = _Jm / C;

    // return the first Piola stress tensor
    return (_mu * B) * F + (2. * _kappa * A * A * A * Jsq_minus_1 - _mu) * transpose(invF);
}

constexpr auto
mito::materials::Gent::tangents(const deformation_gradient_type & F) const -> tangents_type
{
    // precompute useful quantities
    // inverse of deformation gradient
    auto invF = inverse(F);
    // J = det F
    auto detF = determinant(F);
    // J^2 - 1
    auto Jsq_minus_1 = detF * detF - 1.;
    // log(J)
    auto logJ = std::log(detF);
    // tr(C)
    auto trC = trace(transpose(F) * F);
    // (J^2 -1)/2 - log(J)
    auto A = 0.5 * Jsq_minus_1 - logJ;
    // Jm - trC + D /*dim*/
    auto C = (_Jm - trC + D /*dim*/);
    // Jm / (Jm - trC + D /*dim*/))
    auto B = _Jm / C;

    // instantiate a tangent tensor
    auto tangents = tangents_type();

    // multiplies \delta_{ik} \delta_{JL}
    auto coef_0 = _mu * B;
    // multiplies F_{kL} F_{iJ}
    auto coef_1 = 2.0 * _mu * _Jm / (C * C);
    // multiplies F^-1_{Jk} F^-1_{Li}
    auto coef_2 = _mu - 2. * _kappa * A * A * A * Jsq_minus_1;
    // multiplies F^-1_{Ji} F^-1_{Lk}
    auto coef_3 =
        2.0 * _kappa * (3.0 * A * A * Jsq_minus_1 * Jsq_minus_1 + 2.0 * detF * detF * A * A * A);

    // fill the Lagrangian moduli tensor
    for (int i = 0; i < D; i++) {
        for (int J = 0; J < D; J++) {
            tangents[{ i, J, i, J }] += coef_0;
            for (int k = 0; k < D; k++) {
                for (int L = 0; L < D; L++) {
                    tangents[{ i, J, k, L }] += coef_1 * F[{ i, J }] * F[{ k, L }]
                                              + coef_2 * invF[{ J, k }] * invF[{ L, i }]
                                              + coef_3 * invF[{ J, i }] * invF[{ L, k }];
                }
            }
        }
    }

    // return the tangent tensor
    return tangents;
}


// end of file
