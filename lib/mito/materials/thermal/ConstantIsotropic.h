// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// a linear elastic material model
class mito::materials::thermal::ConstantIsotropic {
  public:
    // the dimension of the physical space
    static constexpr int D = 3;
    // the type for a scalar
    using scalar_type = tensor::scalar_t;
    // the type for the deformation gradient
    using deformation_gradient_type = tensor::matrix_t<D>;
    // the type for the stress
    using stress_type = tensor::symmetric_matrix_t<D>;
    // the type for the tangents
    // TOFIX: it would be cool to include the symmetries of the tangent tensor here
    using tangents_type = tensor::fourth_order_tensor_t<D>;

  public:
    // constructor
    constexpr ConstantIsotropic(
        scalar_type kappa, scalar_type rho, scalar_type cp, scalar_type sigma) :
        _kappa(kappa),
        _rho(rho),
        _cp(cp),
        _sigma(sigma)
    {}

  public:
    // returns the thermal conductivity
    constexpr auto conductivity() const -> scalar_type { return _kappa; }

    // returns the volumetric heat capacity
    constexpr auto heat_capacity() const -> scalar_type { return _rho * _cp; }

    // returns the heat source
    constexpr auto heat_source() const -> scalar_type { return _sigma; }

  private:
    // thermal conductivity
    scalar_type _kappa;
    // density
    scalar_type _rho;
    // specific heat
    scalar_type _cp;
    // source term
    scalar_type _sigma;
};


// end of file
