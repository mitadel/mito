// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


namespace mito::manifolds {

    template <class cellT>
    constexpr auto parametric_dim() noexcept -> int;

    template <>
    constexpr auto parametric_dim<topology::segment_t>() noexcept -> int
    {
        return 2;
    }

    template <>
    constexpr auto parametric_dim<topology::triangle_t>() noexcept -> int
    {
        return 3;
    }

    template <>
    constexpr auto parametric_dim<topology::tetrahedron_t>() noexcept -> int
    {
        return 4;
    }
}