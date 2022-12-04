
namespace mito::manifolds {

    template <class cellT>
    constexpr auto parametric_dim() -> int;

    template <>
    constexpr auto parametric_dim<topology::segment_t>() -> int
    {
        return 2;
    }

    template <>
    constexpr auto parametric_dim<topology::triangle_t>() -> int
    {
        return 3;
    }

    template <>
    constexpr auto parametric_dim<topology::tetrahedron_t>() -> int
    {
        return 4;
    }
}