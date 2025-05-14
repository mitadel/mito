// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle} represents a second order simplex equipped parametric coordinates.


namespace mito::discretization {


    template <geometry::geometric_simplex_c geometricSimplexT>
    class IsoparametricTriangle : public utilities::Invalidatable {

      protected:
        // the geometric simplex type
        using geometric_simplex_type = geometricSimplexT;
        // the parametric dimension
        static constexpr int dim = 2;
        // the parametric coordinates type
        using parametric_coordinates_type =
            mito::geometry::coordinates_t<dim, mito::geometry::CARTESIAN>;

      protected:
        // the function extracting the 0 component of a parametric point
        static constexpr auto xi_0 =
            mito::fields::field(mito::functions::component<parametric_coordinates_type, 0>);
        // the function extracting the 1 component of a parametric point
        static constexpr auto xi_1 =
            mito::fields::field(mito::functions::component<parametric_coordinates_type, 1>);
        // the function extracting the 2 component of a parametric point
        static constexpr auto xi_2 = 1.0 - xi_0 - xi_1;

      public:
        // the default constructor
        constexpr IsoparametricTriangle(const geometric_simplex_type & geometric_simplex) :
            _geometric_simplex(geometric_simplex)
        {}

        // destructor
        constexpr ~IsoparametricTriangle() = default;

        // delete move constructor
        constexpr IsoparametricTriangle(IsoparametricTriangle &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangle(const IsoparametricTriangle &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangle & operator=(const IsoparametricTriangle &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangle & operator=(IsoparametricTriangle &&) noexcept = delete;

      public:
        // get the geometric simplex
        constexpr auto geometric_simplex() const noexcept -> const geometric_simplex_type &
        {
            return _geometric_simplex;
        }

      protected:
        // QUESTION: do we need to maintain a reference to the geometric simplex?
        // a const reference to the geometric simplex
        const geometric_simplex_type & _geometric_simplex;
    };

}    // namespace mito


// end of file
