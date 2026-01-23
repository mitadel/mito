// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


template <class materialT>
class mito::materials::MaterialConsistencyTest {
  private:
    // the type of material
    using material_type = materialT;
    // the type for the deformation gradient
    using deformation_gradient_type = materialT::deformation_gradient_type;
    // the type for the stress
    using stress_type = materialT::stress_type;
    // the type for the tangents
    using tangents_type = materialT::tangents_type;

  public:
    // constructor
    MaterialConsistencyTest(
        const material_type & material, const real & perturbation, const real & tolerance) :
        _material(material),
        _perturbation(perturbation),
        _tolerance(tolerance)
    {}

  private:
    auto _energy_increment(const deformation_gradient_type & F) const -> mito::tensor::matrix_t<3>
    {
        // take note of the original deformation gradient
        auto F_perturbed = F;

        // initialize the energy increment tensor
        auto energy_increment = mito::tensor::matrix_t<3>{};

        // for each component of the deformation gradient
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                // make a positive perturbation in direction {i, j}
                F_perturbed[{ i, j }] += _perturbation;
                // compute the energy {q_plus} in the perturbed state
                auto q_plus = _material.energy(F_perturbed);

                // make a negative perturbation in direction {i, j}
                F_perturbed[{ i, j }] -= 2.0 * _perturbation;
                // compute the energy {q_minus} in the perturbed state
                auto q_minus = _material.energy(F_perturbed);

                // compute the increment between the positive and negative perturbations
                energy_increment[{ i, j }] = q_plus - q_minus;

                // go back to the original state {i, j}
                F_perturbed[{ i, j }] = F[{ i, j }];
            }
        }

        // all done
        return energy_increment;
    }

    auto _stress_increment(const deformation_gradient_type & F) const
        -> tensor::fourth_order_tensor_t<3>
    {
        // take note of the original deformation gradient
        auto F_perturbed = F;

        // initialize the stress increment tensor
        auto stress_increment = tensor::fourth_order_tensor_t<3>();

        // for each component of the deformation gradient
        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < 3; l++) {

                // make a positive perturbation in direction {k, l}
                F_perturbed[{ k, l }] += _perturbation;
                // compute the stress {P_plus} in the perturbed state
                auto P_plus = _material.stress(F_perturbed);

                // make a negative perturbation in direction {k, l}
                F_perturbed[{ k, l }] -= 2.0 * _perturbation;
                // compute the stress {P_minus} in the perturbed state
                auto P_minus = _material.stress(F_perturbed);

                // compute the increment between the positive and negative perturbations
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        stress_increment[{ i, j, k, l }] = P_plus[{ i, j }] - P_minus[{ i, j }];
                    }
                }

                // go back to the original state {k, l}
                F_perturbed[{ k, l }] = F[{ k, l }];
            }
        }

        // all done
        return stress_increment;
    }

  public:
    // run the stress consistency test for a deformation gradient {F}
    auto test_stress(const deformation_gradient_type & F) const -> bool
    {
        // make a channel
        journal::info_t channel("tests.stress_consistency_test");

        // report
        channel << "Running the stress consistency test" << journal::endl;

        // compute the analytical stress at {F}
        auto P_analytical = _material.stress(F);

        // compute the energy increment at {F} for a perturbation {_perturbation}
        auto energy_increment = _energy_increment(F);

        // compute the residual
        auto residual = tensor::norm(energy_increment - P_analytical * 2.0 * _perturbation);

        // print the errors
        channel << "First order residual: " << residual << journal::endl;

        // return the outcome of the test
        return residual < _tolerance;
    }

    // run the tangents consistency test for a deformation gradient {F}
    auto test_tangents(const deformation_gradient_type & F) const -> bool
    {
        // make a channel
        journal::info_t channel("tests.tangents_consistency_test");

        // report
        channel << "Running the tangents consistency test" << journal::endl;

        // compute the analytical tangents at {F}
        auto C_analytical = _material.tangents(F);

        // compute the stress increment at {F} for a perturbation {_perturbation}
        auto stress_increment = _stress_increment(F);

        // compute the residual
        auto residual = tensor::norm(stress_increment - C_analytical * 2.0 * _perturbation);

        // print the errors
        channel << "First order residual: " << residual << journal::endl;

        // return the outcome of the test
        return residual < _tolerance;
    }

  private:
    // the material
    const material_type & _material;
    // the perturbation for the numerical derivatives
    const real & _perturbation;
    // the tolerance for passing the test
    const real & _tolerance;
};


// end of file
