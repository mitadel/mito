// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::materials {

    // linear elastic material
    class LinearElastic;

    // linear elastic material alias
    using linear_elastic_t = LinearElastic;

    // gent material
    class Gent;

    // gent material alias
    using gent_t = Gent;

    // material consistency test class
    template <class materialT>
    class MaterialConsistencyTest;

    // material consistency test alias
    template <class materialT>
    using material_consistency_test_t = MaterialConsistencyTest<materialT>;
}


// end of file
