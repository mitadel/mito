// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the parametric coordinates
using parametric_coordinates_type = mito::tensor::vector_t<2>;
constexpr auto xi_0 = mito::functions::component<parametric_coordinates_type, 0>;
constexpr auto xi_1 = mito::functions::component<parametric_coordinates_type, 1>;

// the barycentric coordinates type
using barycentric_coordinates_type = mito::tensor::vector_t<3>;
constexpr auto eta_0 = mito::functions::component<barycentric_coordinates_type, 0>;
constexpr auto eta_1 = mito::functions::component<barycentric_coordinates_type, 1>;
constexpr auto eta_2 = mito::functions::component<barycentric_coordinates_type, 2>;

// the conversion from parametric to barycentric coordinates
constexpr auto parametric_to_barycentric = xi_0 * mito::tensor::e_0<3> + xi_1 * mito::tensor::e_1<3>
                                         + (1.0 - xi_0 - xi_1) * mito::tensor::e_2<3>;

// the conversion from barycentric to parametric coordinates
constexpr auto barycentric_to_parametric =
    eta_0 * mito::tensor::e_0<2> + eta_1 * mito::tensor::e_1<2>;


TEST(Fem, ShapeTriangleConstuctionP1)
{
    // create a channel
    journal::info_t channel("tests.shape_triangle_construction_p1");

    // linear shape functions on the reference triangle in barycentric coordinates
    constexpr auto phi_0 = eta_0;
    constexpr auto phi_1 = eta_1;
    constexpr auto phi_2 = eta_2;

    // hard-coded derivatives of shape functions with respect to parametric coordinates
    constexpr auto dphi_0 =
        mito::functions::constant<barycentric_coordinates_type>(mito::tensor::e_0<2>);
    constexpr auto dphi_1 =
        mito::functions::constant<barycentric_coordinates_type>(mito::tensor::e_1<2>);
    constexpr auto dphi_2 = mito::functions::constant<barycentric_coordinates_type>(
        -mito::tensor::e_0<2> - mito::tensor::e_1<2>);

    // assemble the shape functions gradients as the partial derivatives of the shape functions with
    // respect to the parametric coordinates, seen as functions of barycentric coordinates
    constexpr auto dN0 = mito::functions::derivative(phi_0(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN1 = mito::functions::derivative(phi_1(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN2 = mito::functions::derivative(phi_2(parametric_to_barycentric))(
        barycentric_to_parametric);

    // barycenter in barycentric coordinates
    constexpr auto barycenter = barycentric_coordinates_type{ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 };

    // check result
    static_assert(mito::tensor::row<0>(dN0(barycenter)) == dphi_0(barycenter));
    static_assert(mito::tensor::row<0>(dN1(barycenter)) == dphi_1(barycenter));
    static_assert(mito::tensor::row<0>(dN2(barycenter)) == dphi_2(barycenter));

    // all done
    return;
}


TEST(Fem, ShapeTriangleConstuctionP2)
{
    // create a channel
    journal::info_t channel("tests.shape_triangle_construction_p2");

    // quadratic shape functions on the reference triangle in barycentric coordinates
    constexpr auto phi_3 = 4.0 * eta_0 * eta_1;
    constexpr auto phi_4 = 4.0 * eta_1 * eta_2;
    constexpr auto phi_5 = 4.0 * eta_0 * eta_2;
    constexpr auto phi_0 = eta_0 - 0.5 * phi_5 - 0.5 * phi_3;
    constexpr auto phi_1 = eta_1 - 0.5 * phi_3 - 0.5 * phi_4;
    constexpr auto phi_2 = eta_2 - 0.5 * phi_5 - 0.5 * phi_4;

    // hard-coded derivatives of shape functions with respect to parametric coordinates
    constexpr auto dphi_0 = mito::tensor::e_0<2> * (1.0 - 2.0 * eta_1 - 2.0 * eta_2 + 2.0 * eta_0);
    constexpr auto dphi_1 = mito::tensor::e_1<2> * (1.0 - 2.0 * eta_0 - 2.0 * eta_2 + 2.0 * eta_1);
    constexpr auto dphi_2 = (mito::tensor::e_0<2> + mito::tensor::e_1<2>) *(
        -1.0 - 2.0 * eta_2 + 2.0 * eta_0 + 2.0 * eta_1);
    constexpr auto dphi_3 =
        mito::tensor::e_0<2> * (4.0 * eta_1) + mito::tensor::e_1<2> * (4.0 * eta_0);
    constexpr auto dphi_4 =
        mito::tensor::e_0<2> * (-4.0 * eta_1) + mito::tensor::e_1<2> * (4.0 * eta_2 - 4.0 * eta_1);
    constexpr auto dphi_5 =
        mito::tensor::e_0<2> * (4.0 * eta_2 - 4.0 * eta_0) + mito::tensor::e_1<2> * (-4.0 * eta_0);

    // assemble the shape functions gradients as the partial derivatives of the shape functions with
    // respect to the parametric coordinates, seen as functions of barycentric coordinates
    constexpr auto dN0 = mito::functions::derivative(phi_0(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN1 = mito::functions::derivative(phi_1(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN2 = mito::functions::derivative(phi_2(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN3 = mito::functions::derivative(phi_3(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN4 = mito::functions::derivative(phi_4(parametric_to_barycentric))(
        barycentric_to_parametric);
    constexpr auto dN5 = mito::functions::derivative(phi_5(parametric_to_barycentric))(
        barycentric_to_parametric);

    // barycenter in barycentric coordinates
    constexpr auto barycenter = barycentric_coordinates_type{ 1.0, 0.0, 0.0 };

    // check result
    static_assert(mito::tensor::row<0>(dN0(barycenter)) == dphi_0(barycenter));
    static_assert(mito::tensor::row<0>(dN1(barycenter)) == dphi_1(barycenter));
    static_assert(mito::tensor::row<0>(dN2(barycenter)) == dphi_2(barycenter));
    static_assert(mito::tensor::row<0>(dN3(barycenter)) == dphi_3(barycenter));
    static_assert(mito::tensor::row<0>(dN4(barycenter)) == dphi_4(barycenter));
    static_assert(mito::tensor::row<0>(dN5(barycenter)) == dphi_5(barycenter));

    // all done
    return;
}

// end of file