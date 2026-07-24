// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>

// 2D Cartesian, embedded segment
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
using cell_t = mito::geometry::triangle_t<2>;
using segment_t = mito::geometry::segment_t<2>;
using discretization_node_t = mito::discrete::discretization_node_t;

TEST(Fem, HybridInterfaceBlocksChapter5Matrices)
{
    auto coord_system = coord_system_t();

    // horizontal interface segment from (0,0) to (1,0)
    auto n0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto n1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto geometric_segment = mito::geometry::segment<2>({ n0, n1 });
    auto seg_mesh = mito::mesh::mesh<segment_t>();
    seg_mesh.insert({ n0, n1 });
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.0, 1.0 });
    auto seg_manifold = mito::manifolds::submanifold(seg_mesh, coord_system, normal_field);
    using iface_element_t = mito::fem::isoparametric_simplex_t<1, decltype(seg_manifold)>;
    auto dn0 = discretization_node_t();
    auto dn1 = discretization_node_t();
    auto iface_el = iface_element_t(
        geometric_segment, coord_system, typename iface_element_t::connectivity_type{ dn0, dn1 },
        seg_manifold.volume_form());

    // dummy bulk elements (required for API)
    auto t0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto t1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto t2 = mito::geometry::node(coord_system, { 0.0, 1.0 });
    auto tri_cell = mito::geometry::triangle<2>({ t0, t1, t2 });
    auto tri_mesh = mito::mesh::mesh<cell_t>();
    tri_mesh.insert({ t0, t1, t2 });
    auto tri_manifold = mito::manifolds::manifold(tri_mesh, coord_system);
    using bulk_element_t = mito::fem::isoparametric_simplex_t<1, decltype(tri_manifold)>;
    auto b0 = discretization_node_t();
    auto b1 = discretization_node_t();
    auto b2 = discretization_node_t();
    auto bulk_el = bulk_element_t(
        tri_cell, coord_system, typename bulk_element_t::connectivity_type{ b0, b1, b2 },
        tri_manifold.volume_form());

    const double h = 1.0;
    const double kappa_m_over_w = 2.0;
    const double w_kappa_m_over_4 = 3.0;

    // P1 mass on segment: M = (h/6)*[[2,1],[1,2]]
    const double M00 = h * 2.0 / 6.0;
    const double M01 = h * 1.0 / 6.0;
    // P1 stiffness: G = (1/h)*[[1,-1],[-1,1]]
    const double G00 = 1.0 / h;
    const double G01 = -1.0 / h;

    const double jump_sign[4][4] = {
        { +1, +1, -1, -1 },
        { +1, +1, -1, -1 },
        { -1, -1, +1, +1 },
        { -1, -1, +1, +1 },
    };

    auto jj = mito::fem::blocks::InterfaceJumpJumpMassBlock<bulk_element_t, iface_element_t>(
        kappa_m_over_w);
    auto K_jj = jj.compute(iface_el, bulk_el, bulk_el);

    const double M[2][2] = { { M00, M01 }, { M01, M00 } };
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            const int lr = r % 2;
            const int lc = c % 2;
            const double expected = kappa_m_over_w * jump_sign[r][c] * M[lr][lc];
            const double computed = K_jj[{ r, c }];
            EXPECT_NEAR(expected, computed, 1e-14) << " jj r=" << r << " c=" << c;
        }
    }

    auto aa = mito::fem::blocks::InterfaceAverageAverageGradBlock<bulk_element_t, iface_element_t>(
        w_kappa_m_over_4);
    auto K_aa = aa.compute(iface_el, bulk_el, bulk_el);

    const double G[2][2] = { { G00, G01 }, { G01, G00 } };
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            const int lr = r % 2;
            const int lc = c % 2;
            const double expected = -w_kappa_m_over_4 * G[lr][lc];
            const double computed = K_aa[{ r, c }];
            EXPECT_NEAR(expected, computed, 1e-14) << " aa r=" << r << " c=" << c;
        }
    }

    mito::fem::InterfaceWeakform<bulk_element_t, iface_element_t, 4> wf;
    wf.add_block(jj);
    wf.add_block(aa);
    auto K_sum = wf.compute_blocks(iface_el, bulk_el, bulk_el);
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            const double sum = K_jj[{ r, c }] + K_aa[{ r, c }];
            const double s = K_sum[{ r, c }];
            EXPECT_NEAR(sum, s, 1e-14) << " sum r=" << r << " c=" << c;
        }
    }
}
