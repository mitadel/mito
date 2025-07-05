// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // nodal field
    template <int D, class Y>
    using nodal_field_t = DiscreteField<geometry::node_t<D>, Y>;

    // point field
    template <int D, class Y>
    using point_field_t = DiscreteField<geometry::point_t<D>, Y>;

    // quadrature field alias
    template <class cellT, int Q, class Y>
    using quadrature_field_t = DiscreteField<cellT, std::array<Y, Q>>;

    // quadrature field factory
    template <class Y, int Q, mesh::mesh_c meshT>
    constexpr auto quadrature_field(const meshT & mesh, std::string name);

    // nodal field factory
    template <class Y, int D>
    constexpr auto nodal_field(const mesh::mesh_c auto & mesh, std::string name);

    // point field factory
    template <class Y, geometry::point_cloud_c cloudT>
    constexpr auto point_field(const cloudT & cloud, std::string name);

    // discrete system alias
    template <class functionSpaceT>
    using discrete_system_t = DiscreteSystem<functionSpaceT>;

    // discrete system factory
    template <class functionSpaceT>
    constexpr auto discrete_system(const functionSpaceT & function_space);
}


// end of file
