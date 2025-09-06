// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    class ReferenceTriangle {

      public:
        // the parametric coordinates type
        using parametric_coordinates_type = geometry::coordinates_t<2, geometry::CARTESIAN>;

        // the function extracting the 0 component of a parametric point
        static constexpr auto xi_0 =
            fields::field(functions::component<parametric_coordinates_type, 0>);
        // the function extracting the 1 component of a parametric point
        static constexpr auto xi_1 =
            fields::field(functions::component<parametric_coordinates_type, 1>);
        // the function extracting the 2 component of a parametric point
        static constexpr auto xi_2 = 1.0 - xi_0 - xi_1;
    };

}    // namespace mito


// end of file
