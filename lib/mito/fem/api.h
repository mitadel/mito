// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // quadrature field factory
    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name = "");

    // nodal field factory
    template <class Y, int D>
    constexpr auto nodal_field(const mesh::mesh_c auto & mesh, std::string name);

    // point field factory
    template <class Y, geometry::point_cloud_c cloudT>
    constexpr auto point_field(const cloudT & cloud, std::string name);
}


// end of file
