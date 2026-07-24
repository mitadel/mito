// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include "driver.h"

#include <gtest/gtest.h>
#include <mito.h>

#include <vector>


TEST(Fem, Chapter5HybridDimensionalAnalyticalVerification)
{
    constexpr auto tol_l2 = 1e-12;
    constexpr auto tol_profile = 1e-8;
    constexpr auto tol_row_spread = 1e-8;

    mito::petsc::initialize();

    const auto parameters = hybrid_interface_chapter5_verification::Parameters{};
    const auto output =
        hybrid_interface_chapter5_verification::OutputOptions{ false, false,
                                                               "dg_chapter5_verification_test" };

    auto ratios = std::vector<hybrid_interface_chapter5_verification::scalar_t>{};
    {
        const auto primary = hybrid_interface_chapter5_verification::primary_ratios();
        const auto diagnostic = hybrid_interface_chapter5_verification::diagnostic_ratios();
        ratios.assign(primary.begin(), primary.end());
        ratios.insert(ratios.end(), diagnostic.begin(), diagnostic.end());
    }

    for (const auto ratio : ratios) {
        const auto result =
            hybrid_interface_chapter5_verification::run_case(parameters, ratio, output);

        EXPECT_NEAR(result.conductivity_ratio, ratio, 1e-15)
            << "conductivity ratio mismatch for requested case";

        EXPECT_LT(result.l2_error, tol_l2) << "ratio=" << ratio << " l2_error=" << result.l2_error;
        EXPECT_LT(result.max_profile_error, tol_profile)
            << "ratio=" << ratio << " max_profile_error=" << result.max_profile_error;
        EXPECT_LT(result.max_row_spread, tol_row_spread)
            << "ratio=" << ratio << " max_row_spread=" << result.max_row_spread;
    }

    mito::petsc::finalize();
}
