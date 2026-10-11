// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// a linear elastic material model
template <int D>
class mito::materials::thermal::ConstantIsotropic {
  public:
    // the type of coordinates
    using coordinates_t = geometry::cartesian<D>::coordinates_t;
    // the type for a scalar
    using scalar_type = tensor::scalar_t;

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
    constexpr auto conductivity() const { return _kappa * functions::identity<coordinates_t, D>(); }

    // returns the volumetric heat capacity
    constexpr auto heat_capacity() const { return _rho * _cp * functions::one<coordinates_t>; }

    // returns the heat source
    constexpr auto heat_source() const { return _sigma; }

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
