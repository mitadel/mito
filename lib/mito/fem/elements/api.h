// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // factory of finite element from a parametrized element
    template <class finiteElementTraits, class parametrizedElementT>
    requires compatible_element_type_c<parametrizedElementT, finiteElementTraits>
    constexpr auto finite_element(
        const parametrizedElementT & element,
        const typename finiteElementTraits::connectivity_type & connectivity)
    {
        // get the type of the finite element from the traits
        using finite_element_type =
            typename finiteElementTraits::template type<parametrizedElementT>;

        // assemble the finite element from the parametrized element and the connectivity
        return finite_element_type(element, connectivity);
    }

}


// end of file
