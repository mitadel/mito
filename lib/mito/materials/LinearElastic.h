// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    class LinearElastic {
      private:
        // the dimension of the physical space
        static constexpr int D = 3;
        // the type for a scalar
        using scalar_type = scalar_t;
        // the type for the deformation gradient
        using deformation_gradient_type = matrix_t<D>;
        // the type for the stress
        using stress_type = symmetric_matrix_t<D>;
        // the type for the tangents
        // TOFIX: it would be cool to include the symmetries of the tangent tensor here
        using tangents_type = fourth_order_tensor_t<D>;

      public:
        constexpr LinearElastic(scalar_type rho, scalar_type E, scalar_type nu) :
            _rho(rho),
            _E(E),
            _nu(nu),
            _lambda(_E * _nu / ((1.0 + _nu) * (1.0 - 2.0 * _nu))),
            _mu(_E / (2.0 * (1.0 + _nu)))
        {}

        // returns the strain energy density
        constexpr auto energy(const deformation_gradient_type &) const -> scalar_t;

        // returns the stress tensor
        constexpr auto stress(const deformation_gradient_type &) const -> stress_type;

        // returns the material tangents
        constexpr auto tangents(const deformation_gradient_type &) const -> tangents_type;

      private:
        // density
        scalar_t _rho;
        // Young's modulus
        scalar_t _E;
        // Poisson's ratio
        scalar_t _nu;
        // 1st Lamé parameter
        scalar_t _lambda;
        // 2nd Lamé parameter
        scalar_t _mu;
    };

    constexpr auto LinearElastic::energy(const deformation_gradient_type & Du) const -> scalar_type
    {
        // the linear strain associated with {Du}
        auto epsilon = symmetric(Du - identity<D>);

        // return the Cauchy stress tensor
        return 0.5 * dot(stress(Du), epsilon);
    }

    constexpr auto LinearElastic::stress(const deformation_gradient_type & Du) const -> stress_type
    {
        // the linear strain associated with {Du}
        auto epsilon = symmetric(Du - identity<D>);

        // trace of epsilon
        auto tr = trace(epsilon);

        // return the Cauchy stress tensor
        return _lambda * tr * identity<D> + 2.0 * _mu * epsilon;
    }

    constexpr auto LinearElastic::tangents(const deformation_gradient_type & /*Du*/) const
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

        // all done
        return C;
    }
}


// end of file
