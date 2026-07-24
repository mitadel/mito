// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include "driver.h"

#include <cstdlib>
#include <iostream>
#include <vector>


int
main(int argc, char * argv[])
{
    auto channel = journal::info_t("apps.dg_chapter5_verification");

    mito::petsc::initialize();

    auto parameters = hybrid_interface_chapter5_verification::Parameters{};
    const auto primary_ratios = hybrid_interface_chapter5_verification::primary_ratios();
    auto ratios =
        std::vector<hybrid_interface_chapter5_verification::scalar_t>{ primary_ratios.begin(),
                                                                       primary_ratios.end() };

    bool include_diagnostics = true;
    if (argc > 1) {
        ratios.clear();

        for (int index = 1; index < argc; ++index) {
            const auto argument = std::string(argv[index]);
            if (argument == "--primary-only") {
                include_diagnostics = false;
                continue;
            }

            ratios.push_back(std::atof(argv[index]));
        }
    }

    if (include_diagnostics && argc == 1) {
        const auto diagnostics = hybrid_interface_chapter5_verification::diagnostic_ratios();
        ratios.insert(ratios.end(), diagnostics.begin(), diagnostics.end());
    }

    channel << "Running Chapter 5 DG verification with " << ratios.size() << " conductivity ratios"
            << journal::endl;
    channel << "Two independent bulk spaces coupled through interface integrals (eqs. 4.122, "
               "4.132)"
            << journal::endl;

    for (const auto ratio : ratios) {
        const auto output_options =
            hybrid_interface_chapter5_verification::OutputOptions{ true, true,
                                                                   "dg_chapter5_verification" };
        const auto result =
            hybrid_interface_chapter5_verification::run_case(parameters, ratio, output_options);

        channel << "ratio=" << result.conductivity_ratio << ", L2=" << result.l2_error
                << ", max_profile_error=" << result.max_profile_error
                << ", max_row_spread=" << result.max_row_spread << journal::endl;
    }

    mito::petsc::finalize();

    return 0;
}
