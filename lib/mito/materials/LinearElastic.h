// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// a linear elastic material model
class mito::materials::LinearElastic {
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
    constexpr LinearElastic(scalar_type rho, scalar_type E, scalar_type nu) :
        _rho(rho),
        _E(E),
        _nu(nu),
        _lambda(_E * _nu / ((1.0 + _nu) * (1.0 - 2.0 * _nu))),
        _mu(_E / (2.0 * (1.0 + _nu)))
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
    // Young's modulus
    scalar_type _E;
    // Poisson's ratio
    scalar_type _nu;
    // 1st Lamé parameter
    scalar_type _lambda;
    // 2nd Lamé parameter
    scalar_type _mu;
};

constexpr auto
mito::materials::LinearElastic::energy(const deformation_gradient_type & F) const -> scalar_type
{
    // the linear strain associated with {F}
    auto epsilon = tensor::symmetric(F - tensor::identity<D>);

    // return the strain energy density
    return 0.5 * tensor::dot(stress(F), epsilon);
}

constexpr auto
mito::materials::LinearElastic::stress(const deformation_gradient_type & F) const -> stress_type
{
    // the linear strain associated with {F}
    auto epsilon = tensor::symmetric(F - tensor::identity<D>);

    // trace of epsilon
    auto tr = tensor::trace(epsilon);

    // return the Cauchy stress tensor
    return _lambda * tr * tensor::identity<D> + 2.0 * _mu * epsilon;
}

constexpr auto
mito::materials::LinearElastic::tangents(const deformation_gradient_type & /*F*/) const
    -> tangents_type
{
    // instantiate a tangent tensor
    auto C = tangents_type();

    // fill the tangent tensor
    for (int a = 0; a < D; ++a) {
        for (int b = 0; b < D; ++b) {
            C[{ a, a, b, b }] += _lambda;
            C[{ a, b, a, b }] += _mu;
            C[{ b, a, a, b }] += _mu;
        }
    }

    // return the tangent tensor
    return C;
}


// end of file
